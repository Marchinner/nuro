#include "cursor.h"

#include "../core/utils/log.h"
#include "../core/context/application_context.h"

namespace Cursor {

	GLFWwindow* gWindow;

	void setup()
	{
		gWindow = ApplicationContext::getWindow();
	}

	glm::vec2 getPosition()
	{
		double mouseX = 0.0, mouseY = 0.0;
		glfwGetCursorPos(gWindow, &mouseX, &mouseY);
		return glm::vec2(mouseX, mouseY);
	}

	void setPosition(glm::vec2 position)
	{
		glfwSetCursorPos(gWindow, position.x, position.y);
	}

	void setType(uint32_t cursorType)
	{
		glfwSetCursor(gWindow, glfwCreateStandardCursor(cursorType));
	}

	void setMode(uint32_t cursorMode)
	{
		glfwSetInputMode(gWindow, GLFW_CURSOR, cursorMode);
	}

	void center()
	{
		int32_t windowWidth, windowHeight;
		glfwGetWindowSize(gWindow, &windowWidth, &windowHeight);
		glm::vec2 cursorPosition = glm::vec2(windowWidth / 2.0f, windowHeight / 2.0f);
		glfwSetCursorPos(gWindow, cursorPosition.x, cursorPosition.y);
	}

}
