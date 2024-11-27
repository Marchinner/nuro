#include "pre_pass.h"

#include <glad/glad.h>

#include "../src/runtime/runtime.h"
#include "../src/entity/entity.h"
#include "../src/rendering/core/mesh_renderer.h"
#include "../src/rendering/shader/shader.h"
#include "../src/utils/log.h"

unsigned int PrePass::width = 0;
unsigned int PrePass::height = 0;

unsigned int PrePass::fbo = 0;

unsigned int PrePass::depthOutput = 0;
unsigned int PrePass::normalOutput = 0;

void PrePass::setup(unsigned int width, unsigned int height)
{
	// Initialize members
	PrePass::width = width;
	PrePass::height = height;

	// Generate framebuffer
	glGenFramebuffers(1, &fbo);
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);

	// Generate depth output
	glGenTextures(1, &depthOutput);
	glBindTexture(GL_TEXTURE_2D, depthOutput);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT24, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);

	// Set depth output parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	// Set depth output as rendering target
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthOutput, 0);

	// Generate normal output
	glGenTextures(1, &normalOutput);
	glBindTexture(GL_TEXTURE_2D, normalOutput);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, width, height, 0, GL_RGB, GL_FLOAT, nullptr);

	// Set normal output parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	// Set normal output as rendering target
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, normalOutput, 0);

	// Check for framebuffer errors
	GLenum fboStatus = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	if (fboStatus != GL_FRAMEBUFFER_COMPLETE) {
		Log::printError("Framebuffer", "Error generating bloom fbo: " + std::to_string(fboStatus));
	}

	// Unbind fbo
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void PrePass::render()
{
	// Set viewport for upcoming pre pass
	glViewport(0, 0, width, height);

	// Bind pre pass framebuffer
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);

	// Clear color and depth buffer
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Enable depth testing
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	// Cull backfaces
	glCullFace(GL_BACK);

	// Bind pre pass shader
	Runtime::prePassShader->bind();

	// Pre pass render each entity
	std::vector<Entity*> entityLinks = Runtime::entityLinks;
	for (int i = 0; i < entityLinks.size(); i++) {
		entityLinks[i]->meshRenderer->prePass();
	}
}

unsigned int PrePass::getDepthOutput()
{
	// Return pre pass depth output
	return depthOutput;
}

unsigned int PrePass::getNormalOutput()
{
	// Return pre pass normal output
	return normalOutput;
}

unsigned int PrePass::getWidth()
{
	// Return width
	return width;
}

unsigned int PrePass::getHeight()
{
	// Return height
	return height;
}