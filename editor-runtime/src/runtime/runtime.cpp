#include "runtime.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm.hpp>

#include "../src/ui/editor_ui.h"
#include "../src/example/src/game_logic.h"

#include "../core/rendering/model/model.h"
#include "../core/rendering/shader/shader.h"
#include "../core/rendering/skybox/cubemap.h"
#include "../core/rendering/primitives/quad.h"
#include "../core/rendering/texture/texture.h"
#include "../core/rendering/shader/shader_pool.h"
#include "../core/rendering/shadows/shadow_map.h"
#include "../core/rendering/core/transformation.h"
#include "../core/rendering/shadows/shadow_disk.h"
#include "../core/rendering/material/unlit/unlit_material.h"

#include "../core/utils/log.h"
#include "../core/time/time.h"
#include "../core/input/input.h"
#include "../core/input/cursor.h"
#include "../core/physics/physics.h"
#include "../core/viewport/viewport.h"
#include "../core/ecs/ecs_collection.h"
#include "../core/diagnostics/profiler.h"
#include "../core/diagnostics/diagnostics.h"
#include "../core/context/application_context.h"

namespace Runtime {

	// Application context
	ApplicationContext applicationContext;

	// Pipelines
	SceneViewPipeline gSceneViewPipeline;
	GameViewPipeline gGameViewPipeline;

	// Physics
	PhysicsContext gGamePhysics;

	// Scene gizmos
	IMGizmo gSceneGizmos;

	// Shadow
	ShadowDisk* gMainShadowDisk = nullptr;
	ShadowMap* gMainShadowMap = nullptr;

	// Default assets
	Skybox gDefaultSkybox;

	// Game state management
	RegistryState gSceneState;
	bool gGameRunning = false;
	bool gGamePaused = false;

	//
	//
	// PRIVATE RUNTIME CORE METHODS
	//
	//

	void _loadAssets() {

		// Loading all shaders
		std::vector<std::string> shader_paths = {
			"../resources/shaders/materials",
			"../resources/shaders/postprocessing",
			"../resources/shaders/gizmo",
			"../resources/shaders/passes" };
		ShaderPool::loadAndCompile(shader_paths);

		// Create shadow disk
		uint32_t diskWindowSize = 4;
		uint32_t diskFilterSize = 8;
		uint32_t diskRadius = 5;
		gMainShadowDisk = new ShadowDisk(diskWindowSize, diskFilterSize, diskRadius);

		// Create default shadow map
		bool shadow_map_saved = false;
		gMainShadowMap = new ShadowMap(4096, 4096, 40.0f, 40.0f, 0.3f, 1000.0f);

		// Create default skybox
		Cubemap defaultCubemap = Cubemap::loadByCubemap("../resources/skybox/default/default_night.png");
		gDefaultSkybox = Skybox(defaultCubemap);

		// Set default skybox as current skybox
		gGameViewPipeline.linkSkybox(&gDefaultSkybox);

		// Load gizmo icons
		IconPool::loadAll("../resources/icons");
	}

	void _setupScripts() {

		// Set context for scripts needing window context
		Input::setContext(&applicationContext);
		Cursor::setContext(&applicationContext);

		// Create pipelines
		gSceneViewPipeline.create();
		gGameViewPipeline.create();

		// Create game physics instance
		gGamePhysics.create();

		// Setup scene gizmos
		gSceneGizmos.setup();

		// Create primitives
		Quad::create();

		// Setup engine ui
		EditorUI::setup(&applicationContext);

	}

	void _prepareFrame() {

		// Update time
		Time::step(glfwGetTime());

		// Update diagnostics
		Diagnostics::step();

		// Update input system
		Input::step();

		// Clear frame color
		glClearColor(0.03f, 0.03f, 0.03f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

	}

	void _renderShadows()
	{
		// WARNING
		// 
		// Right now, the shadow passes will use the "current model matrix" from the mesh renderer.
		// As the shadows will be rendered before the other rendering pipelines,
		// which prepare the mesh renderer by among other things calculating and 
		// settings its "current model matrix", the "current model matrix" will always be
		// one frame delayed, ultimately delaying object movement for shadows by one frame
		//
		// Fix: Calculate transform matrices before rendering shadows

		//
		// SHADOW PASS
		// Render shadow map
		//
		Profiler::start("shadow_pass");
		gMainShadowMap->render();
		Profiler::stop("shadow_pass");
	}

	void _renderEditor() {

		Profiler::start("ui_pass");

		EditorUI::newFrame();
		EditorUI::render();

		Profiler::stop("ui_pass");

	}

	void _stepGame() {

		// UPDATE GAME LOGIC
		update();

		// STEP GAME PHYSICS
		gGamePhysics.step(Time::deltaf());

		// SET SCENE VIEW PIPELINE TO UPDATED
		gSceneViewPipeline.setUpdated();

	}

	//
	//
	// RUNTIME BASE METHODS (START & TERMINATE)
	//
	//

	int START_LOOP()
	{
		// CREATE CONTEXT AND LOAD GRAPHICS API //
		ApplicationContext::Configuration config;
		config.api = API::OPENGL;
		config.vsync = false;

		applicationContext.create(config);

		// LOAD ASSETS, COMPILE SHADERS
		_loadAssets();

		// CALL ANY OTHER SCRIPTS NEEDING SETUP
		_setupScripts();

		// PERFORM GAMES SETUP LOGIC
		setup();

		// GENERATE ALL INITIAL QUEUES
		ECS::generateRenderQueue();

		while (applicationContext.running())
		{
			// START NEW APPLICATION CONTEXT FRAME
			applicationContext.startFrame();

			// UPDATE ANY SCRIPTS NEEDING UPDATE FOR NEXT FRAME (Time, Inputs etc.)
			_prepareFrame();

			// UPDATE GAME IF GAME IS RUNNING
			if (gGameRunning && !gGamePaused) _stepGame();

			// RENDER NEXT FRAME (full render pipeline pass)
			_renderShadows();
			gSceneViewPipeline.tryRender();
			gGameViewPipeline.tryRender();

			// RENDER EDITOR
			_renderEditor();

			// END CURRENT FRAME
			applicationContext.endFrame();
		}

		// Exit application

		return TERMINATE();
	}

	int TERMINATE()
	{
		// Destroy all instances
		gSceneViewPipeline.destroy();
		gGameViewPipeline.destroy();
		gGamePhysics.destroy();

		// Destroy context
		applicationContext.destroy();

		// Exit application
		std::exit(0);

		return 0;
	}

	void startGame()
	{
		// Re-Generate render queue

		// Cache scene state
		// gSceneState = ECS::captureState();

		// Perform awake logic
		awake();

		// Set game running state
		gGameRunning = true;
		gGamePaused = false;
	}

	void stopGame()
	{
		// Perform quit logic
		quit();

		// Set game running state
		gGameRunning = false;
		gGamePaused = false;

		// Restore scene state
		// ECS::loadState(gSceneState);
	}

	void pauseGame() {
		// Set game to paused
		gGamePaused = true;
	}

	void continueGame() {
		// Set game to not paused anymore
		gGamePaused = false;
	}

	bool gameRunning()
	{
		return gGameRunning;
	}

	bool gamePaused()
	{
		return gGamePaused;
	}

	SceneViewPipeline& getSceneViewPipeline()
	{
		return gSceneViewPipeline;
	}

	GameViewPipeline& getGameViewPipeline()
	{
		return gGameViewPipeline;
	}

	PhysicsContext& getGamePhysics()
	{
		return gGamePhysics;
	}

	IMGizmo& getSceneGizmos()
	{
		return gSceneGizmos;
	}

	PhysicsContext& getPhysicsContext()
	{
		return gGamePhysics;
	}

	ShadowDisk* getMainShadowDisk()
	{
		return gMainShadowDisk;
	}

	ShadowMap* getMainShadowMap()
	{
		return gMainShadowMap;
	}

}