#include "game_logic.h"

#include <iostream>

#include "../engine/runtime.h"
#include "../engine/objects/entity.h"
#include "../engine/objects/camera.h"
#include "../engine/rendering/material/unlit_material.h"
#include "../engine/rendering/material/rainbow_material.h"

Camera* camera = nullptr;
Entity* cube = nullptr;
Entity* floorLamp = nullptr;
Entity* mannequin = nullptr;

int amount = 100;
std::vector<Entity*> object_batch(amount);

void awake() {
	// Create camera
	camera = new Camera();
	Runtime::useCamera(camera);

	// Import textures
	Texture* dirtTexture = new Texture("./user/assets/textures/dirt.jpg");

	UnlitMaterial* unlit = new UnlitMaterial();
	RainbowMaterial* rainbow = new RainbowMaterial();

	Model* cubeModel = new Model("./user/assets/models/cube.obj", unlit);
	Model* floorLampModel = new Model("./user/assets/models/floor_lamp.fbx", unlit);
	Model* mannequinModel = new Model("./user/assets/models/mannequin.fbx", rainbow);

	cube = Runtime::createEntity();
	cube->model = cubeModel;
	cube->position = glm::vec3(0.0f, 0.0f, 7.5f);
	cube->scale = glm::vec3(0.75f, 0.75f, 0.75f);

	floorLamp = Runtime::createEntity();
	floorLamp->model = floorLampModel;
	floorLamp->position = glm::vec3(-1.0f, 0.0f, 5.0f);
	floorLamp->rotation = glm::vec3(90.0f, 0.0f, 90.0f);
	floorLamp->scale = glm::vec3(1.5f, 1.5f, 1.5f);

	mannequin = Runtime::createEntity();
	mannequin->model = mannequinModel;
	mannequin->position = glm::vec3(1.0f, 0.0f, 5.0f);
	mannequin->rotation = glm::vec3(90.0f, 0.0f, 0.0f);
}

void update() {
	mannequin->rotation.z += 0.5f;
}