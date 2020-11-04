#include "Window.h"
#include <iostream>
#include "input/Mouse.h"
#include "input/Keyboard.h"
#include "gameobjects/GameObject.h"

Window* Window::CurrentWindow = nullptr;

Window::Window(const std::string& aTitle, const int& aScreenWidth, const int& aScreenHeight)
	: myTitle(aTitle), myScreenWidth(aScreenWidth), myScreenHeight(aScreenHeight), myRawWindow(nullptr)
{
	// Load OpenGL functions
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
}

Window::~Window()
{
}

void Window::Run()
{
	Window::CurrentWindow = this;
	Mouse::BindToWindow(myRawWindow);

	float tempLastTime = 0.0f;
	float tempLastFPSTime = 0.0f;
	int tempElapsedFrames = 0;

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glfwSetInputMode(myRawWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// OnStart (called once at the start of the program)
	OnStart();
	Window::OnStart();

	while (!glfwWindowShouldClose(myRawWindow))
	{
		// Timing
		float tempCurrentTime = glfwGetTime();
		float tempDeltaTime = tempCurrentTime - tempLastTime;
		tempLastTime = tempCurrentTime;

		// FPS (Frames Per Second)
		tempElapsedFrames++;
		if (tempCurrentTime - tempLastFPSTime >= 1.0f)
		{
			myFPS = tempElapsedFrames;

			tempLastFPSTime += 1.0f;
			tempElapsedFrames = 0;
		}

		// OnUpdate (called every frame)
		OnUpdate(tempDeltaTime);
		Window::OnUpdate(tempDeltaTime);

		glClearColor(0.392f, 0.584f, 0.929f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// OnRender (called every frame)
		OnRender(tempDeltaTime);
		Window::OnRender(tempDeltaTime);

		Keyboard::UpdateKeys(myRawWindow);
		Mouse::UpdateButtons(myRawWindow);

		glfwSwapBuffers(myRawWindow);
		glfwPollEvents();
	}

	glfwTerminate();
}

void Window::OnStart()
{

}

void Window::OnUpdate(const float& aDeltaTime)
{
	
}

void Window::OnRender(const float& aDeltaTime)
{

}