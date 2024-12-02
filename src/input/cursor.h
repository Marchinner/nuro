#pragma once

#include <glm.hpp>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace Cursor
{

	void setContext(GLFWwindow* window); // Set context for cursor system

	glm::vec2 getPosition(); // Return the current cursor position relative to window
	void setPosition(glm::vec2 position); // Set the cursor position relative to window

	void setType(int cursorType); // Set cursor type
	void setMode(int cursorMode); // Set cursor mode

	void center(); // Center the cursor relative to window

};

namespace CursorType {
	static constexpr int DEFAULT = GLFW_ARROW_CURSOR;
	static constexpr int POINTER = GLFW_HAND_CURSOR;
	static constexpr int TEXT = GLFW_IBEAM_CURSOR;
	static constexpr int CROSSHAIR = GLFW_CROSSHAIR_CURSOR;
	static constexpr int RESIZE_HORIZONTAL = GLFW_HRESIZE_CURSOR;
	static constexpr int RESIZE_VERTICAL = GLFW_VRESIZE_CURSOR;
};

namespace CursorMode
{
	static constexpr int NORMAL = GLFW_CURSOR_NORMAL;
	static constexpr int HIDDEN = GLFW_CURSOR_HIDDEN;
	static constexpr int UNAVAILABLE = GLFW_CURSOR_UNAVAILABLE;
	static constexpr int CONFINED = GLFW_CURSOR_CAPTURED;
	static constexpr int LOCKED = GLFW_CURSOR_DISABLED;
};