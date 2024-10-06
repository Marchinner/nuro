#include "game_logic.h"

#include <iostream>

#include "../engine/runtime/runtime.h"
#include "../engine/entity/entity.h"
#include "../engine/camera/camera.h"
#include "../engine/rendering/material/unlit_material.h"
#include "../engine/rendering/material/lit_material.h"

Camera* camera = nullptr;
Entity* cube = nullptr;
Entity* floorLamp = nullptr;
Entity* mannequin = nullptr;

LitMaterial* lit;
LitMaterial* dirt;

int amount = 100;
std::vector<Entity*> object_batch(amount);

void awake() {
	// Set default skybox
	Runtime::setSkybox(Runtime::defaultSkybox);

	// Create camera
	camera = new Camera();
	Runtime::useCamera(camera);

	// Import textures
	Texture* dirtTexture = new Texture("./user/assets/textures/dirt.jpg");

	UnlitMaterial* unlit = new UnlitMaterial(Runtime::defaultDiffuseTexture);

	/*LitMaterial* lightGray = new LitMaterial(Runtime::defaultDiffuseTexture);
	lightGray->baseColor = glm::vec4(0.7f, 0.7f, 0.7f, 1.0f);
	LitMaterial* darkGray = new LitMaterial(Runtime::defaultDiffuseTexture);
	darkGray->baseColor = glm::vec4(0.85f, 0.85f, 0.85f, 1.0f);
	LitMaterial* white = new LitMaterial(Runtime::defaultDiffuseTexture);
	white->baseColor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
	LitMaterial* black = new LitMaterial(Runtime::defaultDiffuseTexture);
	black->baseColor = glm::vec4(0.5f, 0.5f, 0.5f, 1.0f);*/

	lit = new LitMaterial(Runtime::defaultDiffuseTexture);
	dirt = new LitMaterial(dirtTexture);

	Model* cubeModel = new Model("./user/assets/models/cube.obj", { lit, lit });
	cube = new Entity();
	cube->model = cubeModel;
	cube->position = glm::vec3(-2.0f, 0.0f, 3.5f);

	LitMaterial* floorMaterial = new LitMaterial(Runtime::defaultDiffuseTexture);
	floorMaterial->baseColor = glm::vec4(0.6f, 0.6f, 0.6f, 1.0f);
	Model* floorModel = new Model("./user/assets/models/cube.obj", { floorMaterial, floorMaterial });
	Entity* floor = new Entity();
	floor->model = floorModel;
	floor->position = glm::vec3(0.0f, -1.0f, 0.0f);
	floor->scale = glm::vec3(25.0f, 0.1f, 25.0f);
	
	//Model* floorLampModel = new Model("./user/assets/models/floor_lamp.fbx", { lightGray, black, darkGray, white });
	/*floorLamp = Runtime::createEntity();
	floorLamp->model = floorLampModel;
	floorLamp->position = glm::vec3(-1.0f, 0.0f, 5.0f);
	floorLamp->rotation = glm::vec3(90.0f, 0.0f, 90.0f);
	floorLamp->scale = glm::vec3(1.5f, 1.5f, 1.5f);*/

	Model* mannequinModel = new Model("./user/assets/models/mannequin.fbx", lit);
	mannequin = new Entity();
	mannequin->model = mannequinModel;
	mannequin->position = glm::vec3(1.0f, -0.9f, 5.5f);
	mannequin->rotation = glm::vec3(90.0f, 0.0f, 0.0f);
}

void update() {
	// Object rotation
	cube->rotation.y += 1.0f;
	mannequin->rotation.z += 1.0f;
}