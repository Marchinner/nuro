#pragma once

#include "../engine/rendering/shader/shader.h"
#include "../engine/rendering/shader/shader_builder.h"
#include "../engine/rendering/texture/texture.h"
#include "../engine/utils/log.h"
#include "../imaterial.h"

class LitMaterial : public IMaterial
{
public:
	LitMaterial();
	void bind();
	Shader* getShader();

	glm::vec4 baseColor;
	Texture* diffuseMap;
private:
	Shader* shader;
};

