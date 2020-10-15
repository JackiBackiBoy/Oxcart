#include "Mouse.h"

double Mouse::myPositionX = 0;
double Mouse::myPositionY = 0;
double Mouse::myScrollOffsetX = 0;
double Mouse::myScrollOffsetY = 0;

GLFWwindow* Mouse::myWindow = nullptr;

Mouse::Mouse()
{
}

void Mouse::ScrollCallback(GLFWwindow* aWindow, double anXOffset, double aYOffset)
{
	myScrollOffsetX = anXOffset;
	myScrollOffsetY = aYOffset;
}

Mouse::~Mouse()
{

}

float Mouse::GetScrollWheel()
{
	double tempCurrentOffsetY = myScrollOffsetY;
	myScrollOffsetY = 0;

	if (myWindow != nullptr && tempCurrentOffsetY != 0)
	{
		
		return (float)tempCurrentOffsetY;
	}

	return 0;
}

Vector2D Mouse::GetPosition(GLFWwindow* aWindow)
{
	glfwGetCursorPos(aWindow, &myPositionX, &myPositionY);

	return { (float)myPositionX, (float)myPositionY };
}

void Mouse::BindToWindow(GLFWwindow* aWindow)
{
	myWindow = aWindow;
	glfwSetScrollCallback(myWindow, ScrollCallback);
}