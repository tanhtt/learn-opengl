#pragma once

#include "GLFW/glfw3.h"

class InputManager {
private:
	GLFWwindow* window;
public:
	InputManager(GLFWwindow* win);
	~InputManager();

	float GetAxisX(float moveAmount) const;
	float GetAxisY(float moveAmount) const;
	float AdjustScale(float currentScale) const;
	float AdjustRocation(float currentRotate) const;
};
