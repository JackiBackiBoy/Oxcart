#include "GL/glew.h"
#include "GLFW/glfw3.h"

#include <cmath>
#include <iostream>
#include "math/Vector4D.h"
#include "graphics/Shader.h"
#include "graphics/Texture.h"

#include "vendor/glm/glm.hpp"
#include "vendor/glm/gtc/matrix_transform.hpp"
#include "vendor/glm/gtc/type_ptr.hpp"

#include "input/Keyboard.h"
#include "input/Mouse.h"
#include "core/Window.h"

glm::vec3 myCameraPosition = glm::vec3(0.0f, 0.0f, 3.0f);
glm::vec3 myCameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
glm::vec3 myCameraFront = glm::vec3(0.0f, 0.0f, -1.0f);

// Light
glm::vec3 myLightPosition = glm::vec3(1.2f, 1.0f, 2.0f);
glm::vec2 myLastMousePosition;

glm::mat4 myViewMatrix = glm::lookAt(glm::vec3(0.0f, 0.0f, 3.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));

float tempDeltaTime = 0.0f;
float lastFrameTime = 0.0f;

float myPitch;
float myYaw = -90.0f;
float myRoll;

extern Window* BuildWindow();

int main(void)
{
	Window* tempWindow = BuildWindow();
	tempWindow->Run();
	delete tempWindow;
	return 0;

	GLFWwindow* window;

	/* Initialize the library */
	if (!glfwInit())
		return -1;

	/* Create a windowed mode window and its OpenGL context */
	int myScreenWidth = 1920;
	int myScreenHeight = 1080;
	float myAspectRatio = (float)myScreenWidth / myScreenHeight;

	window = glfwCreateWindow(myScreenWidth, myScreenHeight, "Oxcart 3D - Sandbox | By Jack Henrikson", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		return -1;
	}

	/* Make the window's context current */
	glfwMakeContextCurrent(window);

	if (glewInit() != GLEW_OK)
	{
		std::cout << "Error: GLEW couldn't be loaded!" << std::endl;
	}

	Shader myLightingShader = { "res/shaders/LightingShader.glsl" };
	Shader myLightCubeShader = { "res/shaders/LightCubeShader.glsl" };

	Texture myWoodBoxTexture = { "res/textures/wood_box.png" };
	Texture myWoodBoxSpecularMap = { "res/textures/wood_box_specular_map.png" };

	float tempVertices[] =
	{
		// positions          // normals           // texture coords
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
		 0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,

		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,
		 0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,

		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
		-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

		 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
		 0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 1.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,

		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f
	};

	glm::vec3 cubePositions[] =
	{
		glm::vec3(0.0f,  0.0f,  0.0f),
		glm::vec3(2.0f,  5.0f, -15.0f),
		glm::vec3(-1.5f, -2.2f, -2.5f),
		glm::vec3(-3.8f, -2.0f, -12.3f),
		glm::vec3(2.4f, -0.4f, -3.5f),
		glm::vec3(-1.7f,  3.0f, -7.5f),
		glm::vec3(1.3f, -2.0f, -2.5f),
		glm::vec3(1.5f,  2.0f, -2.5f),
		glm::vec3(1.5f,  0.2f, -1.5f),
		glm::vec3(-1.3f,  1.0f, -1.5f)
	};

	glm::vec3 pointLightPositions[] =
	{
		glm::vec3(0.7f,  0.2f,  2.0f),
		glm::vec3(2.3f, -3.3f, -4.0f),
		glm::vec3(-4.0f,  2.0f, -12.0f),
		glm::vec3(0.0f,  0.0f, -3.0f)
	};

	unsigned int tempIndices[] = { 0, 1, 2, 0, 2, 3/*,
								   4, 5, 6, 4, 6, 7*/ };

	// Vertex Buffer Object (VBO)
	unsigned int myVBO; // VBO
	glGenBuffers(1, &myVBO);
	glBindBuffer(GL_ARRAY_BUFFER, myVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(tempVertices), tempVertices, GL_STATIC_DRAW);

	// Vertex Array Object (VAO)
	unsigned int myVAO;
	glGenVertexArrays(1, &myVAO);
	glBindVertexArray(myVAO);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), 0);
	glEnableVertexAttribArray(0);

	// Normal
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	// Texture coords
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	unsigned int myLightVAO;
	glGenVertexArrays(1, &myLightVAO);
	glBindVertexArray(myLightVAO);

	glBindBuffer(GL_ARRAY_BUFFER, myVBO);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), 0);
	glEnableVertexAttribArray(0);

	glEnable(GL_DEPTH_TEST);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window))
	{
		float tempCurrentFrameTime = glfwGetTime();
		tempDeltaTime = tempCurrentFrameTime - lastFrameTime;
		lastFrameTime = tempCurrentFrameTime;

		// Forward
		if (Keyboard::IsKeyDown(KeyCode::W, window))
		{
			myCameraPosition += 2.0f * tempDeltaTime * myCameraFront;
		}
		// Backward
		if (Keyboard::IsKeyDown(KeyCode::S, window))
		{
			myCameraPosition -= 2.0f * tempDeltaTime * myCameraFront;
		}

		// Left
		if (Keyboard::IsKeyDown(KeyCode::A, window))
		{
			myCameraPosition -= glm::normalize(glm::cross(myCameraFront, myCameraUp)) * 2.0f * tempDeltaTime;
		}
		// Right
		if (Keyboard::IsKeyDown(KeyCode::D, window))
		{
			myCameraPosition += glm::normalize(glm::cross(myCameraFront, myCameraUp)) * 2.0f * tempDeltaTime;
		}

		// Up
		if (Keyboard::IsKeyDown(KeyCode::Space, window))
		{
			myCameraPosition.y += 2.0f * tempDeltaTime;
		}
		// Down
		if (Keyboard::IsKeyDown(KeyCode::LeftControl, window))
		{
			myCameraPosition.y -= 2.0f * tempDeltaTime;
		}

		Vector2D tempCurrentMousePosition = Mouse::GetPosition(window);

		float tempDeltaMouseX = tempCurrentMousePosition.x - myLastMousePosition.x;
		float tempDeltaMouseY = myLastMousePosition.y - tempCurrentMousePosition.y;
		//myLastMousePosition = tempCurrentMousePosition;

		myYaw += tempDeltaMouseX * 0.1f;
		myPitch += tempDeltaMouseY * 0.1f;

		if (myPitch > 89.0f)
		{
			myPitch = 89.0f;
		}
		if (myPitch < -89.0f)
		{
			myPitch = -89.0f;
		}

		// Horizontal mouse movement
		glm::vec3 tempDirection;
		tempDirection.x = cos(glm::radians(myYaw)) * cos(glm::radians(myPitch));
		tempDirection.y = sin(glm::radians(myPitch));
		tempDirection.z = sin(glm::radians(myYaw)) * cos(glm::radians(myPitch));
		myCameraFront = glm::normalize(tempDirection);

		/* Render here */
		glClearColor(0.4f, 0.4f, 0.4f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glUseProgram(myLightingShader.GetID());
		glUniform3f(glGetUniformLocation(myLightingShader.GetID(), "light.position"), myCameraPosition.x, myCameraPosition.y, myCameraPosition.z);
		glUniform3f(glGetUniformLocation(myLightingShader.GetID(), "directionalLight.direction"), -1.0f, -0.23f, -0.9f);
		glUniform1f(glGetUniformLocation(myLightingShader.GetID(), "light.cutOff"), glm::cos(glm::radians(12.5f)));
		glUniform1f(glGetUniformLocation(myLightingShader.GetID(), "light.outerCutOff"), glm::cos(glm::radians(17.5f)));

		// Spotlight
		glUniform3f(glGetUniformLocation(myLightingShader.GetID(), "spotLight.position"), myCameraPosition.x, myCameraPosition.y, myCameraPosition.z);
		glUniform3f(glGetUniformLocation(myLightingShader.GetID(), "spotLight.direction"), myCameraFront.x, myCameraFront.y, myCameraFront.z);
		glUniform1f(glGetUniformLocation(myLightingShader.GetID(), "spotLight.cutOff"), glm::cos(glm::radians(12.5f)));
		glUniform1f(glGetUniformLocation(myLightingShader.GetID(), "spotLight.outerCutOff"), glm::cos(glm::radians(17.5f)));
		glUniform3f(glGetUniformLocation(myLightingShader.GetID(), "spotLight.ambient"), 0.1f, 0.1f, 0.1f);
		glUniform3f(glGetUniformLocation(myLightingShader.GetID(), "spotLight.diffuse"), 0.8f, 0.8f, 0.8f);
		glUniform3f(glGetUniformLocation(myLightingShader.GetID(), "spotLight.specular"), 1.0f, 1.0f, 1.0f);
		glUniform1f(glGetUniformLocation(myLightingShader.GetID(), "spotLight.constantTerm"), 1.0f);
		glUniform1f(glGetUniformLocation(myLightingShader.GetID(), "spotLight.linearTerm"), 0.09f);
		glUniform1f(glGetUniformLocation(myLightingShader.GetID(), "spotLight.quadraticTerm"), 0.032f);

		glUniform3f(glGetUniformLocation(myLightingShader.GetID(), "ViewPosition"), myCameraPosition.x, myCameraPosition.y, myCameraPosition.z);

		myWoodBoxTexture.Bind(0);
		myWoodBoxSpecularMap.Bind(1);

		glUniform1i(glGetUniformLocation(myLightingShader.GetID(), "material.diffuse"), 0);
		glUniform1i(glGetUniformLocation(myLightingShader.GetID(), "material.specular"), 1);
		glUniform1f(glGetUniformLocation(myLightingShader.GetID(), "material.shininess"), 32.0f);
		glUniform3f(glGetUniformLocation(myLightingShader.GetID(), "directionalLight.ambient"), 0.2f, 0.2f, 0.2f);
		glUniform3f(glGetUniformLocation(myLightingShader.GetID(), "directionalLight.diffuse"), 0.5f, 0.5f, 0.5f);
		glUniform3f(glGetUniformLocation(myLightingShader.GetID(), "directionalLight.specular"), 1.0f, 1.0f, 1.0f);
		glUniform1f(glGetUniformLocation(myLightingShader.GetID(), "light.constantTerm"), 1.0f);
		glUniform1f(glGetUniformLocation(myLightingShader.GetID(), "light.linearTerm"), 0.09f);
		glUniform1f(glGetUniformLocation(myLightingShader.GetID(), "light.quadraticTerm"), 0.032f);

		// Model Matrix
		glm::mat4 tempModelMatrix = glm::mat4(1.0f);
		tempModelMatrix = glm::rotate(tempModelMatrix, 0.0f, glm::vec3(1.0f, 1.0f, 1.0f));

		// View Matrix
		glm::mat4 tempViewMatrix;
		tempViewMatrix = glm::lookAt(myCameraPosition, myCameraPosition + myCameraFront, myCameraUp);

		// Projection Matrix
		glm::mat4 tempProjectionMatrix;
		tempProjectionMatrix = glm::perspective(glm::radians(45.0f), myAspectRatio, 0.1f, 100.0f);

		glUniformMatrix4fv(glGetUniformLocation(myLightingShader.GetID(), "ModelMatrix"), 1, GL_FALSE, glm::value_ptr(tempModelMatrix));
		glUniformMatrix4fv(glGetUniformLocation(myLightingShader.GetID(), "ViewMatrix"), 1, GL_FALSE, glm::value_ptr(tempViewMatrix));
		glUniformMatrix4fv(glGetUniformLocation(myLightingShader.GetID(), "ProjectionMatrix"), 1, GL_FALSE, glm::value_ptr(tempProjectionMatrix));

		glBindVertexArray(myVAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		for (size_t i = 0; i < 10; i++)
		{
			glm::mat4 model = glm::mat4(1.0f);
			model = glm::translate(model, cubePositions[i]);
			float angle = 20.0f * i;
			model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
			glUniformMatrix4fv(glGetUniformLocation(myLightingShader.GetID(), "ModelMatrix"), 1, GL_FALSE, glm::value_ptr(model));

			glDrawArrays(GL_TRIANGLES, 0, 36);
		}

		// Draw the scene light
		glUseProgram(myLightCubeShader.GetID());
		glUniformMatrix4fv(glGetUniformLocation(myLightCubeShader.GetID(), "ProjectionMatrix"), 1, GL_FALSE, glm::value_ptr(tempProjectionMatrix));
		glUniformMatrix4fv(glGetUniformLocation(myLightCubeShader.GetID(), "ViewMatrix"), 1, GL_FALSE, glm::value_ptr(tempViewMatrix));
		tempModelMatrix = glm::mat4(1.0f);
		tempModelMatrix = glm::translate(tempModelMatrix, myLightPosition);
		tempModelMatrix = glm::scale(tempModelMatrix, glm::vec3(0.2f));
		glUniformMatrix4fv(glGetUniformLocation(myLightCubeShader.GetID(), "ModelMatrix"), 1, GL_FALSE, glm::value_ptr(tempModelMatrix));

		glBindVertexArray(myLightVAO);

		for (size_t i = 0; i < 4; i++)
		{

		}

		glDrawArrays(GL_TRIANGLES, 0, 36);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glDeleteVertexArrays(1, &myVAO);
	glDeleteVertexArrays(1, &myLightVAO);
	glDeleteBuffers(1, &myVBO);

	glfwTerminate();

	return 0;
}