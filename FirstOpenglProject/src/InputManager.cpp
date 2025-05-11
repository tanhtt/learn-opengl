#include "InputManager.h"
#include <iostream>

InputManager::InputManager(GLFWwindow* win)
	:window(win) {}

InputManager::~InputManager()
{
}

float InputManager::GetAxisX(float moveAmount) const
{
	if (glfwGetKey(window, GLFW_KEY_RIGHT)) return moveAmount;
	if (glfwGetKey(window, GLFW_KEY_LEFT)) return - moveAmount;
	return 0.0f;
}

float InputManager::GetAxisY(float moveAmount) const
{
	if (glfwGetKey(window, GLFW_KEY_UP)) return moveAmount;
	if (glfwGetKey(window, GLFW_KEY_DOWN)) return -moveAmount;
	return 0.0f;
}

float InputManager::AdjustScale(float currentScale) const
{
	if (glfwGetKey(window, GLFW_KEY_KP_ADD) || glfwGetKey(window, GLFW_KEY_EQUAL)) {
		return std::min(currentScale + 0.01f, 3.0f);
	}
	if (glfwGetKey(window, GLFW_KEY_KP_SUBTRACT) || glfwGetKey(window, GLFW_KEY_MINUS)) {
		return std::max(currentScale - 0.01f, 0.2f);
	}
	return currentScale;
}

float InputManager::AdjustRocation(float currentRotate) const
{
	if (glfwGetKey(window, GLFW_KEY_X)) return currentRotate + 0.5f;
	if (glfwGetKey(window, GLFW_KEY_Z)) return currentRotate - 0.5f;
	return currentRotate;
}




