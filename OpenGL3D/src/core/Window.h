#ifndef WINDOW_HEADER
#define WINDOW_HEADER

#include <string>
#include "GL/glew.h"
#include <GLFW/glfw3.h>

class Window
{
public:
	Window(const std::string& aTitle, const int& aScreenWidth, const int& aScreenHeight);
	virtual ~Window();

	void Run();

	virtual void OnStart() = 0;
	virtual void OnUpdate(const float& aDeltaTime) = 0;
	virtual void OnRender(const float& aDeltaTime) = 0;

protected:
	std::string myTitle;
	int myScreenWidth;
	int myScreenHeight;

	GLFWwindow* myRawWindow;
};
#endif