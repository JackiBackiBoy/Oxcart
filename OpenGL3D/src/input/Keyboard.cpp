#include "Keyboard.h"

Keyboard::Keyboard()
{

}

Keyboard::~Keyboard()
{

}

bool Keyboard::IsKeyDown(KeyCode aKeyCode, GLFWwindow* aWindow)
{
	if (glfwGetKey(aWindow, (int)aKeyCode) == GLFW_PRESS)
	{
		return true;
	}

	return false;
}