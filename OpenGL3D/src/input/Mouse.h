#ifndef MOUSE_HEADER
#define MOUSE_HEADER

#include "math/Vector2D.h"
#include "GLFW/glfw3.h"

class Mouse
{
public:
	~Mouse();

	static Vector2D GetPosition(GLFWwindow* aWindow);

private:
	Mouse();

	static double myPositionX;
	static double myPositionY;
};
#endif