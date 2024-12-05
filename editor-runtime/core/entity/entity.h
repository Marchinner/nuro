#pragma once

#include "../core/transform/transform.h"
#include "../core/rendering/core/mesh_renderer.h"

struct Entity
{
	Entity();

	Transform transform;
	MeshRenderer meshRenderer;
};