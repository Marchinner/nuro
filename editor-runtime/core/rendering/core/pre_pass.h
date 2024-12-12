#pragma once

#include <vector>

#include "../core/viewport/viewport.h"
#include "../core/rendering/shader/shader.h"

class OldEntity;

class PrePass
{
public:
	PrePass(const Viewport& viewport);
	
	void create();
	void destroy();

	void render(std::vector<OldEntity*>& targets);

	unsigned int getDepthOutput();
	unsigned int getNormalOutput();

private:
	const Viewport& viewport;

	unsigned int fbo;
	unsigned int depthOutput;
	unsigned int normalOutput;

	Shader* prePassShader;
};