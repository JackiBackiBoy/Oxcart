#ifndef WINDOW_HEADER
#define WINDOW_HEADER

#include <string>
#include "GL/glew.h"
#include <GLFW/glfw3.h>
#include "math/Vector2D.h"

class GameObject;

class Window
{
public:
	Window(const std::string& aTitle, const int& aScreenWidth, const int& aScreenHeight);
	virtual ~Window();

	void Run();
	virtual void OnStart() = 0;
	virtual void OnUpdate(const float& aDeltaTime) = 0;
	virtual void OnRender(const float& aDeltaTime) = 0;

	inline float GetScreenWidth() const { return myScreenWidth; }
	inline float GetScreenHeight() const { return myScreenHeight; }
	inline float GetAspectRatio() const { return (float)myScreenWidth / myScreenHeight; }
	inline int GetFPS() { return myFPS; }
	inline GLFWwindow* GetRawWindow() const { return myRawWindow; }

	inline Vector2D GetCenter() const { return { (float)myScreenWidth / 2, (float)myScreenHeight / 2 }; }

	static Window* CurrentWindow;

private:
	int myFPS;

protected:
	std::string myTitle;
	int myScreenWidth;
	int myScreenHeight;

	GLFWwindow* myRawWindow;
};
#endif