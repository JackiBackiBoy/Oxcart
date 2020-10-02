#include "core/Window.h"
#include "components/Camera.h"
#include "math/Math.h"
#include "math/Vector2D.h"
#include "math/Matrix4x4.h"

#include "input/Keyboard.h"
#include "input/Mouse.h"

#include "graphics/Shader.h"
#include "graphics/Texture.h"

#include "vendor/glm/glm.hpp"
#include "vendor/glm/gtc/matrix_transform.hpp"
#include "vendor/glm/gtc/type_ptr.hpp"

#include "data/Model.h"

#include <iostream>

class Sandbox : public Window
{
public: 
	Sandbox(const std::string& aTitle, const int& aScreenWidth, const int& aScreenHeight)
		: Window::Window(aTitle, aScreenWidth, aScreenHeight) {}

	void OnStart() override
	{
		char tempPath[] = "C:\\Code\\Oxcart\\OpenGL3D\\res\\models\\backpack.obj";
		aModel = Model(tempPath);

		//myCamera = new Camera();
		//myCamera->GetTransform().Position() = { 0.0f, 0.0f, 3.0f };
		//myCamera->GetTransform().Rotation() = { 0.0f, 0.0f, -1.0f };

		myModelShader = Shader("res/shaders/ModelLoading.glsl");
		myLightingShader = new Shader("res/shaders/LightingShader.glsl");
		myLightCubeShader = new Shader("res/shaders/LightCubeShader.glsl");

		myWoodBoxTexture = new Texture("res/textures/wood_box.png");
		myWoodBoxSpecularMap = new Texture("res/textures/wood_box_specular_map.png");

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

		Vector3D pointLightPositions[] =
		{
			Vector3D(0.7f,  0.2f,  2.0f),
			Vector3D(2.3f, -3.3f, -4.0f),
			Vector3D(-4.0f,  2.0f, -12.0f),
			Vector3D(0.0f,  0.0f, -3.0f)
		};

		unsigned int tempIndices[] = { 0, 1, 2, 0, 2, 3/*,
								   4, 5, 6, 4, 6, 7*/ };

								   // Vertex Buffer Object (VBO)
		unsigned int myVBO; // VBO
		glGenBuffers(1, &myVBO);
		glBindBuffer(GL_ARRAY_BUFFER, myVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(tempVertices), tempVertices, GL_STATIC_DRAW);

		// Vertex Array Object (VAO)
		myVAO;
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

		myLightVAO;
		glGenVertexArrays(1, &myLightVAO);
		glBindVertexArray(myLightVAO);

		glBindBuffer(GL_ARRAY_BUFFER, myVBO);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), 0);
		glEnableVertexAttribArray(0);

		glEnable(GL_DEPTH_TEST);
		glfwSetInputMode(myRawWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	}

	void OnUpdate(const float& aDeltaTime) override
	{
		// Forward
		if (Keyboard::IsKeyDown(KeyCode::W, myRawWindow))
		{
			myCameraPosition += myCameraFront * 2.0f * aDeltaTime;
		}
		// Backward
		if (Keyboard::IsKeyDown(KeyCode::S, myRawWindow))
		{
			myCameraPosition -= myCameraFront * 2.0f * aDeltaTime;
		}

		// Left
		if (Keyboard::IsKeyDown(KeyCode::A, myRawWindow))
		{
			myCameraPosition -= Vector3D::Normalize(Vector3D::CrossProduct(myCameraFront, myCameraUp)) * 2.0f * aDeltaTime;
		}
		// Right
		if (Keyboard::IsKeyDown(KeyCode::D, myRawWindow))
		{
			myCameraPosition += Vector3D::Normalize(Vector3D::CrossProduct(myCameraFront, myCameraUp)) * 2.0f * aDeltaTime;
		}

		// Up
		if (Keyboard::IsKeyDown(KeyCode::Space, myRawWindow))
		{
			myCameraPosition.y += 2.0f * aDeltaTime;
		}
		// Down
		if (Keyboard::IsKeyDown(KeyCode::LeftControl, myRawWindow))
		{
			myCameraPosition.y -= 2.0f * aDeltaTime;
		}

		Vector2D tempCurrentMousePosition = Mouse::GetPosition(myRawWindow);

		float tempDeltaMouseX = tempCurrentMousePosition.x - myLastMousePosition.x;
		float tempDeltaMouseY = myLastMousePosition.y - tempCurrentMousePosition.y;
		myLastMousePosition = tempCurrentMousePosition;

		myYaw += tempDeltaMouseX * 0.1f;
		myPitch += tempDeltaMouseY * 0.1f;

		if (myPitch > 89.0f)
		{
			myPitch = 89.0f;
		}
		if (myPitch < -89.0f)
		{
			myPitch= -89.0f;
		}

		Vector3D tempDirection;
		tempDirection.x = cos(Math::ToRadians(myYaw)) * cos(Math::ToRadians(myPitch));
		tempDirection.y = sin(Math::ToRadians(myPitch));
		tempDirection.z = sin(Math::ToRadians(myYaw)) * cos(Math::ToRadians(myPitch));
		myCameraFront = Vector3D::Normalize(tempDirection);
	}

	void OnRender(const float& aDeltaTime) override
	{
		glUseProgram(myLightingShader->GetID());
		glUniform3f(glGetUniformLocation(myLightingShader->GetID(), "light.position"), myCameraPosition.x, myCameraPosition.y, myCameraPosition.z);
		glUniform3f(glGetUniformLocation(myLightingShader->GetID(), "directionalLight.direction"), -1.0f, -0.23f, -0.9f);
		glUniform1f(glGetUniformLocation(myLightingShader->GetID(), "light.cutOff"), cos(Math::ToRadians(12.5f)));
		glUniform1f(glGetUniformLocation(myLightingShader->GetID(), "light.outerCutOff"), cos(Math::ToRadians(17.5f)));

		// Spotlight
		glUniform3f(glGetUniformLocation(myLightingShader->GetID(), "spotLight.position"), myCameraPosition.x, myCameraPosition.y, myCameraPosition.z);
		glUniform3f(glGetUniformLocation(myLightingShader->GetID(), "spotLight.direction"), myCameraFront.x, myCameraFront.y, myCameraFront.z);
		glUniform1f(glGetUniformLocation(myLightingShader->GetID(), "spotLight.cutOff"), cos(Math::ToRadians(12.5f)));
		glUniform1f(glGetUniformLocation(myLightingShader->GetID(), "spotLight.outerCutOff"), cos(Math::ToRadians(17.5f)));
		glUniform3f(glGetUniformLocation(myLightingShader->GetID(), "spotLight.ambient"), 0.1f, 0.1f, 0.1f);
		glUniform3f(glGetUniformLocation(myLightingShader->GetID(), "spotLight.diffuse"), 0.8f, 0.8f, 0.8f);
		glUniform3f(glGetUniformLocation(myLightingShader->GetID(), "spotLight.specular"), 1.0f, 1.0f, 1.0f);
		glUniform1f(glGetUniformLocation(myLightingShader->GetID(), "spotLight.constantTerm"), 1.0f);
		glUniform1f(glGetUniformLocation(myLightingShader->GetID(), "spotLight.linearTerm"), 0.09f);
		glUniform1f(glGetUniformLocation(myLightingShader->GetID(), "spotLight.quadraticTerm"), 0.032f);

		glUniform3f(glGetUniformLocation(myLightingShader->GetID(), "ViewPosition"), myCameraPosition.x, myCameraPosition.y, myCameraPosition.z);

		myWoodBoxTexture->Bind(0);
		myWoodBoxSpecularMap->Bind(1);

		glUniform1i(glGetUniformLocation(myLightingShader->GetID(), "material.diffuse"), 0);
		glUniform1i(glGetUniformLocation(myLightingShader->GetID(), "material.specular"), 1);
		glUniform1f(glGetUniformLocation(myLightingShader->GetID(), "material.shininess"), 32.0f);
		glUniform3f(glGetUniformLocation(myLightingShader->GetID(), "directionalLight.ambient"), 0.2f, 0.2f, 0.2f);
		glUniform3f(glGetUniformLocation(myLightingShader->GetID(), "directionalLight.diffuse"), 0.5f, 0.5f, 0.5f);
		glUniform3f(glGetUniformLocation(myLightingShader->GetID(), "directionalLight.specular"), 1.0f, 1.0f, 1.0f);
		glUniform1f(glGetUniformLocation(myLightingShader->GetID(), "light.constantTerm"), 1.0f);
		glUniform1f(glGetUniformLocation(myLightingShader->GetID(), "light.linearTerm"), 0.09f);
		glUniform1f(glGetUniformLocation(myLightingShader->GetID(), "light.quadraticTerm"), 0.032f);

		// Model Matrix
		Matrix4x4 tempModelMatrix = Matrix4x4::Identity();
		tempModelMatrix = Matrix4x4::Rotate(tempModelMatrix, 0.0f, { 1.0f, 1.0f, 1.0f });

		// View Matrix
		Matrix4x4 tempViewMatrix;
		tempViewMatrix = Matrix4x4::LookAt(myCameraPosition, myCameraPosition + myCameraFront, myCameraUp);

		// Projection Matrix
		Matrix4x4 tempProjectionMatrix;
		tempProjectionMatrix = Matrix4x4::Perspective(Math::ToRadians(45.0f), myAspectRatio, 0.1f, 100.0f);

		glUniformMatrix4fv(glGetUniformLocation(myLightingShader->GetID(), "ModelMatrix"), 1, GL_FALSE, tempModelMatrix.GetValuePtr());
		glUniformMatrix4fv(glGetUniformLocation(myLightingShader->GetID(), "ViewMatrix"), 1, GL_FALSE, tempViewMatrix.GetValuePtr());
		glUniformMatrix4fv(glGetUniformLocation(myLightingShader->GetID(), "ProjectionMatrix"), 1, GL_FALSE, tempProjectionMatrix.GetValuePtr());

		glBindVertexArray(myVAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		for (size_t i = 0; i < 10; i++)
		{
			Matrix4x4 model = Matrix4x4::Identity();
			model = Matrix4x4::Translate(model, cubePositions[i]);

			float angle = 20.0f * i;
			model = Matrix4x4::Rotate(model, Math::ToRadians(angle), { 1.0f, 0.3f, 0.5f });
			
			glUniformMatrix4fv(glGetUniformLocation(myLightingShader->GetID(), "ModelMatrix"), 1, GL_FALSE, model.GetValuePtr());

			//glDrawArrays(GL_TRIANGLES, 0, 36);
		}

		// Draw the scene light
		glUseProgram(myLightCubeShader->GetID());
		glUniformMatrix4fv(glGetUniformLocation(myLightCubeShader->GetID(), "ProjectionMatrix"), 1, GL_FALSE, tempProjectionMatrix.GetValuePtr());
		glUniformMatrix4fv(glGetUniformLocation(myLightCubeShader->GetID(), "ViewMatrix"), 1, GL_FALSE, tempViewMatrix.GetValuePtr());

		tempModelMatrix = Matrix4x4::Identity();
		tempModelMatrix = Matrix4x4::Translate(tempModelMatrix, myLightPosition);
		tempModelMatrix = Matrix4x4::Scale(tempModelMatrix, { 0.2f, 0.2f, 0.2f });

		glUniformMatrix4fv(glGetUniformLocation(myLightCubeShader->GetID(), "ModelMatrix"), 1, GL_FALSE, tempModelMatrix.GetValuePtr());

		glBindVertexArray(myLightVAO);

		for (size_t i = 0; i < 4; i++)
		{

		}

		glDrawArrays(GL_TRIANGLES, 0, 36);
		
		glUseProgram(myModelShader.GetID());
		glUniformMatrix4fv(glGetUniformLocation(myModelShader.GetID(), "projection"), 1, GL_FALSE, tempProjectionMatrix.GetValuePtr());
		glUniformMatrix4fv(glGetUniformLocation(myModelShader.GetID(), "view"), 1, GL_FALSE, tempViewMatrix.GetValuePtr());
		glUniformMatrix4fv(glGetUniformLocation(myModelShader.GetID(), "model"), 1, GL_FALSE, tempModelMatrix.GetValuePtr());

		aModel.Render(myModelShader);
	}

private:
	//Camera* myCamera;
	Vector2D myLastMousePosition;

	Vector3D myCameraPosition = { 0.0f, 0.0f, 3.0f };
	Vector3D myCameraUp = { 0.0f, 1.0f, 0.0f };
	Vector3D myCameraFront = { 0.0f, 0.0f, -1.0f };

	Vector3D cubePositions[10] =
	{
		Vector3D(0.0f,  0.0f,  0.0f),
		Vector3D(2.0f,  5.0f, -15.0f),
		Vector3D(-1.5f, -2.2f, -2.5f),
		Vector3D(-3.8f, -2.0f, -12.3f),
		Vector3D(2.4f, -0.4f, -3.5f),
		Vector3D(-1.7f,  3.0f, -7.5f),
		Vector3D(1.3f, -2.0f, -2.5f),
		Vector3D(1.5f,  2.0f, -2.5f),
		Vector3D(1.5f,  0.2f, -1.5f),
		Vector3D(-1.3f,  1.0f, -1.5f)
	};

	unsigned int myVAO;
	unsigned int myLightVAO;

	int myScreenWidth = 1920;
	int myScreenHeight = 1080;
	float myAspectRatio = (float)myScreenWidth / myScreenHeight;

	float myPitch;
	float myYaw = -90.0f;
	float myRoll;

	Shader* myLightingShader;
	Shader* myLightCubeShader;
	Shader myModelShader;

	Vector3D myLightPosition = { 1.2f, 1.0f, 2.0f };

	Texture* myWoodBoxTexture;
	Texture* myWoodBoxSpecularMap;

	Model aModel;
};

Window* BuildWindow()
{
	return new Sandbox("Oxcart 3D | Made by Jack Henrikson", 1920, 1080);
}