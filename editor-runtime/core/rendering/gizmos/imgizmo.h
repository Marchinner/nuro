#pragma once

#include <cstdint>
#include <vector>
#include <glm.hpp>

#include "../core/rendering/texture/texture.h"
#include "../core/ecs/components.h"

class Shader;
class Model;

class IMGizmo
{
public:
	IMGizmo();

	void setup();    // Load gizmo shaders and models if not loaded already
	void newFrame(); // Clear the render stack
	void render(const glm::mat4& viewProjection);   // Render all gizmos from render stack

	// Global render state settings
	glm::vec3 color; // Color of gizmo
	float opacity; // Opacity of gizmo
	bool foreground; // If gizmo should always be in foreground

	// Shapes
	void plane(glm::vec3 position, glm::vec3 scale = glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3 rotation = glm::vec3(0.0f, 0.0f, 0.0f));
	void box(glm::vec3 position, glm::vec3 scale = glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3 rotation = glm::vec3(0.0f, 0.0f, 0.0f));
	void sphere(glm::vec3 position, float radius = 0.5f);
	void planeWire(glm::vec3 position, glm::vec3 scale = glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3 rotation = glm::vec3(0.0f, 0.0f, 0.0f));
	void boxWire(glm::vec3 position, glm::vec3 scale = glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3 rotation = glm::vec3(0.0f, 0.0f, 0.0f));
	void sphereWire(glm::vec3 position, float radius = 0.5f);

	// Icons
	void icon3d(Texture& icon, glm::vec3 position, TransformComponent& cameraTransform, glm::vec3 scale = glm::vec3(1.0f));

	Model* getCubeModel(); // tmp

private:
	struct StaticData {
		bool loaded;

		Shader* fillShader;
		Shader* iconShader;

		Mesh planeMesh;
		Mesh cubeMesh;
		Mesh sphereMesh;
	};

	struct RenderState {
		glm::vec3 color;
		float opacity;
		bool foreground;
	};

	enum class Shape
	{
		PLANE,
		BOX,
		SPHERE
	};

	struct ShapeRenderTarget
	{
		Shape shape;
		glm::vec3 position;
		glm::vec3 rotation;
		glm::vec3 scale;
		bool wireframe;
		RenderState state;

		ShapeRenderTarget(Shape shape, glm::vec3 position, glm::vec3 rotation, glm::vec3 scale, bool wireframe, RenderState state) :
			shape(shape),
			position(position),
			rotation(rotation),
			scale(scale),
			wireframe(wireframe),
			state(state)
		{};
	};

	struct IconRenderTarget
	{
		Texture& icon;
		glm::vec3 position;
		glm::vec3 scale;
		TransformComponent& cameraTransform;
		RenderState state;

		IconRenderTarget(Texture& icon, glm::vec3 position, glm::vec3 scale, TransformComponent& cameraTransform, RenderState state) :
			icon(icon),
			position(position),
			scale(scale),
			cameraTransform(cameraTransform),
			state(state)
		{};
	};

private:
	std::vector<ShapeRenderTarget> shapeRenderStack;
	void renderShapes(const glm::mat4& viewProjection);

	std::vector<IconRenderTarget> iconRenderStack;
	void renderIcons(const glm::mat4& viewProjection);

	// Initialized once by any instance being setup, these will be needed for the rest of the application lifetime
	static StaticData staticData;

private:
	RenderState getCurrentState();
	Mesh& getMesh(Shape shape);
	glm::mat4 getModelMatrix(glm::vec3 position, glm::quat rotation, glm::vec3 scale);
	glm::mat4 getModelMatrix(glm::vec3 position, glm::vec3 rotation, glm::vec3 scale);
};