#ifndef MOUSE_HEADER
#define MOUSE_HEADER

#include "MouseButton.h"
#include "math/Vector2D.h"
#include "GLFW/glfw3.h"

class Mouse
{
public:
	~Mouse();

	static bool IsButtonDownOnce(MouseButton aButton, GLFWwindow* aWindow);
	static bool IsButtonDown(MouseButton aButton, GLFWwindow* aWindow);

	static float GetScrollWheel();
	static Vector2D GetPosition(GLFWwindow* aWindow);

	static void UpdateButtons(GLFWwindow* aWindow);
	static void BindToWindow(GLFWwindow* aWindow);

private:
	Mouse();

	static void ScrollCallback(GLFWwindow* aWindow, double anXOffset, double aYOffset);

	static double myPositionX;
	static double myPositionY;
	static double myScrollOffsetX;
	static double myScrollOffsetY;
	static GLFWwindow* myWindow;
	static bool myButtonStates[8];
};
#endif