#include "core/Window.h"
#include "components/Camera.h"
#include "math/Math.h"
#include "math/Vector2D.h"
#include "math/Matrix4x4.h"

#include "input/Keyboard.h"
#include "input/Mouse.h"

#include "ui/UIText.h"
#include "graphics/Shader.h"

#include "vendor/glm/glm.hpp"
#include "vendor/glm/gtc/matrix_transform.hpp"
#include "vendor/glm/gtc/type_ptr.hpp"
#include "ui/Character.h"

#include "data/Model.h"

#include <map>

#include <iostream>

class Sandbox : public Window
{
public: 
	Sandbox(const std::string& aTitle, const int& aScreenWidth, const int& aScreenHeight)
		: Window::Window(aTitle, aScreenWidth, aScreenHeight) {}

	void OnStart() override
	{
		myLightingShader = new Shader("res/shaders/LightingShader.glsl");
		myLightCubeShader = new Shader("res/shaders/LightCubeShader.glsl");
		myText = new UIText("Beep beep boop", { 10, 10 }, { 255, 255, 255 });

		glEnable(GL_DEPTH_TEST);
		glfwSetInputMode(myRawWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

		aModel = Model("res/models/Nanosuit/nanosuit.obj");
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
		tempProjectionMatrix = Matrix4x4::Perspective(Math::ToRadians(70.0f), GetAspectRatio(), 0.1f, 100.0f);

		glUniformMatrix4fv(glGetUniformLocation(myLightingShader->GetID(), "ModelMatrix"), 1, false, tempModelMatrix.GetValuePtr());
		glUniformMatrix4fv(glGetUniformLocation(myLightingShader->GetID(), "ViewMatrix"), 1, false, tempViewMatrix.GetValuePtr());
		glUniformMatrix4fv(glGetUniformLocation(myLightingShader->GetID(), "ProjectionMatrix"), 1, false, tempProjectionMatrix.GetValuePtr());

		aModel.Render(*myLightingShader);

		// Font rendering
		myText->Render(*this);
	}

private:
	std::map<char, Character> myCharacters;

	Vector2D myLastMousePosition;

	Vector3D myCameraPosition = { 0.0f, 0.0f, 3.0f };
	Vector3D myCameraUp = { 0.0f, 1.0f, 0.0f };
	Vector3D myCameraFront = { 0.0f, 0.0f, -1.0f };

	unsigned int myVAO;
	unsigned int myVBO;

	int myLargestCharacterSize;

	float myPitch;
	float myYaw = -90.0f;
	float myRoll;



	UIText* myText;

	Shader* myLightingShader;
	Shader* myLightCubeShader;
	Shader myGlyphShader;

	Vector3D myLightPosition = { 1.2f, 1.0f, 2.0f };
	Model aModel;
};

Window* BuildWindow()
{
	return new Sandbox("Oxcart | Made by Jack Henrikson", 1920, 1080);
}