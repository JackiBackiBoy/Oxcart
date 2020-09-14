#include "Mouse.h"

double Mouse::myPositionX = 0;
double Mouse::myPositionY = 0;

Mouse::Mouse()
{

}

Mouse::~Mouse()
{

}

Vector2D Mouse::GetPosition(GLFWwindow* aWindow)
{
	glfwGetCursorPos(aWindow, &myPositionX, &myPositionY);

	return { (float)myPositionX, (float)myPositionY };
}