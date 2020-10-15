#include "Keyboard.h"

std::unordered_map<int, bool> Keyboard::myKeyStates;

Keyboard::Keyboard()
{
	for (unsigned int i = 32; i < 348; i++)
	{
		myKeyStates.insert(std::pair<int, bool>(i, false));
	}
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

bool Keyboard::IsKeyDownOnce(KeyCode aKeyCode, GLFWwindow* aWindow)
{
	bool tempHasKeyBeenDown = myKeyStates[(int)aKeyCode];

	if (!glfwGetKey(aWindow, (int)aKeyCode) && tempHasKeyBeenDown)
	{
		return true;
	}

	return false;
}

void Keyboard::UpdateKeys(GLFWwindow* aWindow)
{
	for (unsigned int i = 32; i < 348; i++)
	{
		myKeyStates[i] = glfwGetKey(aWindow, i);
	}
}