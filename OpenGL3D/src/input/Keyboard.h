#ifndef KEYBOARD_HEADER
#define KEYBOARD_HEADER

#include "input/KeyCode.h"
#include <GLFW/glfw3.h>

class Keyboard
{
public:
	~Keyboard();

	static bool IsKeyDown(KeyCode aKeyCode, GLFWwindow* aWindow);

private:
	Keyboard();
};
#endif