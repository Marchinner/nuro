#pragma once

#include <glm.hpp>
#include <gtc/matrix_access.hpp>
#include <gtc/type_ptr.hpp>

#include "../src/transform/transform.h"

struct Frustum
{
	glm::vec4 planes[6];
};

class Camera
{
public:
	Camera();

	Transform transform;

	Frustum frustum;

	float getFov();
	float getNear();
	float getFar();

	void updateFrustum(glm::mat4 viewMatrix, glm::mat4 projectionMatrix);
private:
	float fov;
	float near;
	float far;
};