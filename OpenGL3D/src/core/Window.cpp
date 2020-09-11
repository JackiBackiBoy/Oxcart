#include "Window.h"
#include <iostream>

Window::Window(const std::string& aTitle, const int& aScreenWidth, const int& aScreenHeight)
	: myTitle(aTitle), myScreenWidth(aScreenWidth), myScreenHeight(aScreenHeight), myRawWindow(nullptr)
{

}

Window::~Window()
{
}

void Window::Run()
{
	if (!glfwInit())
	{
		exit(-1);
	}

	/* Create a windowed mode window and its OpenGL context */
	myRawWindow = glfwCreateWindow(myScreenWidth, myScreenHeight, myTitle.c_str(), NULL, NULL);
	if (!myRawWindow)
	{
		glfwTerminate();
		exit(-1);
	}

	/* Make the window's context current */
	glfwMakeContextCurrent(myRawWindow);

	if (glewInit() != GLEW_OK)
	{
		std::cout << "Error: GLEW couldn't be loaded!" << std::endl;
	}

	float tempLastTime = 0.0f;

	// OnStart (called once at the start of the program)
	OnStart();
	Window::OnStart();

	while (!glfwWindowShouldClose(myRawWindow))
	{
		glClearColor(0.4f, 0.4f, 0.4f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		float tempCurrentTime = glfwGetTime();
		float tempDeltaTime = tempCurrentTime - tempLastTime;
		float tempLastTime = tempCurrentTime;

		// OnUpdate (called every frame)
		OnUpdate(tempDeltaTime);
		Window::OnRender(tempDeltaTime);

		// OnRenderer (called every frame)
		OnRender(tempDeltaTime);
		Window::OnRender(tempDeltaTime);

		glfwSwapBuffers(myRawWindow);
		glfwPollEvents();
	}

	glfwTerminate();
}

void Window::OnStart()
{

}

void Window::OnUpdate(float aDeltaTime)
{

}

void Window::OnRender(float aDeltaTime)
{

}