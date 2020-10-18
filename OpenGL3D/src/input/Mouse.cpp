#include "Mouse.h"

Mouse::Mouse()
{
}

Mouse::~Mouse()
{

}

bool Mouse::IsButtonDownOnce(MouseButton aButton, GLFWwindow* aWindow)
{
	// Check if the button was held down in the last frame, and then released
	if (!glfwGetMouseButton(aWindow, (int)aButton) && myButtonStates[(int)aButton])
	{
		return true;
	}

	return false;
}

bool Mouse::IsButtonDown(MouseButton aButton, GLFWwindow* aWindow)
{
	if (glfwGetMouseButton(aWindow, (int)aButton))
	{
		return true;
	}

	return false;
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

void Mouse::UpdateButtons(GLFWwindow* aWindow)
{
	for (int i = 0; i < 8; i++)
	{
		myButtonStates[i] = glfwGetMouseButton(aWindow, i);
	}
}

void Mouse::BindToWindow(GLFWwindow* aWindow)
{
	myWindow = aWindow;
	glfwSetScrollCallback(myWindow, ScrollCallback);
}

void Mouse::ScrollCallback(GLFWwindow* aWindow, double anXOffset, double aYOffset)
{
	myScrollOffsetX = anXOffset;
	myScrollOffsetY = aYOffset;
}

double Mouse::myPositionX = 0;
double Mouse::myPositionY = 0;
double Mouse::myScrollOffsetX = 0;
double Mouse::myScrollOffsetY = 0;
bool Mouse::myButtonStates[8] = { false };
GLFWwindow* Mouse::myWindow = nullptr;