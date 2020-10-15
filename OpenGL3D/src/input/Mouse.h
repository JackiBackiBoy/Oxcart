#ifndef MOUSE_HEADER
#define MOUSE_HEADER

#include "math/Vector2D.h"
#include "GLFW/glfw3.h"

class Mouse
{
public:
	~Mouse();

	static float GetScrollWheel();
	static Vector2D GetPosition(GLFWwindow* aWindow);
	static void BindToWindow(GLFWwindow* aWindow);

private:
	Mouse();

	static void ScrollCallback(GLFWwindow* aWindow, double anXOffset, double aYOffset);

	static double myPositionX;
	static double myPositionY;
	static double myScrollOffsetX;
	static double myScrollOffsetY;
	static GLFWwindow* myWindow;
};
#endif