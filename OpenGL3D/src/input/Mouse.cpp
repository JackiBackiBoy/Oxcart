#include "Mouse.h"

double Mouse::myPositionX = 0;
double Mouse::myPositionY = 0;

Mouse::Mouse()
{

}

Mouse::~Mouse()
{

}

glm::vec2 Mouse::GetPosition(GLFWwindow* aWindow)
{
	glfwGetCursorPos(aWindow, &myPositionX, &myPositionY);

	return { myPositionX, myPositionY };
}