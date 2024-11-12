#pragma once

#include "../engine/rendering/shader/Shader.h"

class BloomPass
{
public:
	static void setup();
	static unsigned int render(unsigned int input);

	static float threshold;
	static float softThreshold;
	static float filterRadius;
private:
	static unsigned int prefilteringPass(unsigned int input);
	static void downsamplingPass(unsigned int input);
	static void upsamplingPass();

	static glm::vec2 fViewportSize;
	static glm::ivec2 iViewportSize;

	static Shader* prefilterShader;
	static Shader* downsamplingShader;
	static Shader* upsamplingShader;
};