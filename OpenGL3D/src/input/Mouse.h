#ifndef MOUSE_HEADER
#define MOUSE_HEADER

#include "vendor/glm/glm.hpp"
#include "GLFW/glfw3.h"

class Mouse
{
public:
	~Mouse();

	static glm::vec2 GetPosition(GLFWwindow* aWindow);

private:
	Mouse();

	static double myPositionX;
	static double myPositionY;
};
#endif