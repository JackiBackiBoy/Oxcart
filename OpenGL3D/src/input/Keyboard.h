#ifndef KEYBOARD_HEADER
#define KEYBOARD_HEADER

#include "input/KeyCode.h"
#include <GLFW/glfw3.h>
#include <unordered_map>

class Keyboard
{
public:
	~Keyboard();

	static bool IsKeyDown(KeyCode aKeyCode, GLFWwindow* aWindow);
	static bool IsKeyDownOnce(KeyCode aKeyCode, GLFWwindow* aWindow);

	static void UpdateKeys(GLFWwindow* aWindow);

private:
	Keyboard();

	static std::unordered_map<int, bool> myKeyStates;
};
#endif