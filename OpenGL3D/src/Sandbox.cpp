#include "core/Window.h"
#include "components/Camera.h"
#include "math/Math.h"
#include "math/Vector2D.h"
#include "math/Matrix4x4.h"

#include "input/Keyboard.h"
#include "input/Mouse.h"

#include "ui/UIText.h"
#include "ui/UIImage.h"
#include "graphics/Shader.h"

#include "data/Model.h"

#include <iostream>

class Sandbox : public Window
{
public: 
	Sandbox(const std::string& aTitle, const int& aScreenWidth, const int& aScreenHeight)
		: Window(aTitle, aScreenWidth, aScreenHeight) {}

	void OnStart() override
	{
		myLightingShader = new Shader("res/shaders/LightingShader.glsl");
		myLightCubeShader = new Shader("res/shaders/LightCubeShader.glsl");
		myUIImageShader = new Shader("res/shaders/UIImageShader.glsl");

		myText = new UIText("Beep beep boop", { 10, 10 }, { 255, 255, 255 });
		myFOV = 70.0f;

		myTexture = new Texture("res/textures/wood_box.png");
		myImage = new UIImage(*myTexture, { 0, 0 });
		myImage->myShader = *myUIImageShader;

		glEnable(GL_DEPTH_TEST);
		glEnable(GL_BLEND);
		glfwSwapInterval(0);
		glfwSetInputMode(myRawWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

		aModel = Model("res/models/Backpack/backpack.obj", true);
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
		myLightingShader->Use();

		myLightingShader->SetUniform3f("light.position", myCameraPosition.x, myCameraPosition.y, myCameraPosition.z);
		myLightingShader->SetUniform3f("directionalLight.direction", -1.0f, -0.23f, -0.9f);
		myLightingShader->SetUniform1f("light.cutOff", cos(Math::ToRadians(12.5f)));
		myLightingShader->SetUniform1f("light.outerCutOff", cos(Math::ToRadians(17.5f)));

		// Spotlight
		myLightingShader->SetUniform3f("spotLight.position", myCameraPosition.x, myCameraPosition.y, myCameraPosition.z);
		myLightingShader->SetUniform3f("spotLight.direction", myCameraFront.x, myCameraFront.y, myCameraFront.z);
		myLightingShader->SetUniform1i("spotLight.cutOff", cos(Math::ToRadians(12.5f)));
		myLightingShader->SetUniform1i("spotLight.outerCutOff", cos(Math::ToRadians(17.5f)));
		myLightingShader->SetUniform3f("spotLight.ambient", 0.1f, 0.1f, 0.1f);
		myLightingShader->SetUniform3f("spotLight.diffuse", 0.8f, 0.8f, 0.8f);
		myLightingShader->SetUniform3f("spotLight.specular", 1.0f, 1.0f, 1.0f);
		myLightingShader->SetUniform1f("spotLight.constantTerm", 1.0f);
		myLightingShader->SetUniform1f("spotLight.linearTerm", 0.09f);
		myLightingShader->SetUniform1f("spotLight.quadraticTerm", 0.032f);

		myLightingShader->SetUniform3f("ViewPosition", myCameraPosition.x, myCameraPosition.y, myCameraPosition.z);

		myLightingShader->SetUniform1i("material.diffuse", 0);
		myLightingShader->SetUniform1i("material.specular", 1);
		myLightingShader->SetUniform1f("material.shininess", 32.0f);
		myLightingShader->SetUniform3f("directionalLight.ambient", 0.2f, 0.2f, 0.2f);
		myLightingShader->SetUniform3f("directionalLight.diffuse", 0.5f, 0.5f, 0.5f);
		myLightingShader->SetUniform3f("directionalLight.specular", 1.0f, 1.0f, 1.0f);
		myLightingShader->SetUniform1f("light.constantTerm", 1.0f);
		myLightingShader->SetUniform1f("light.linearTerm", 0.09f);
		myLightingShader->SetUniform1f("light.quadraticTerm", 0.032f);

		// Model Matrix
		Matrix4x4 tempModelMatrix = Matrix4x4::Identity();
		tempModelMatrix = Matrix4x4::Rotate(tempModelMatrix, 0.0f, { 1.0f, 1.0f, 1.0f });

		// View Matrix
		Matrix4x4 tempViewMatrix;
		tempViewMatrix = Matrix4x4::LookAt(myCameraPosition, myCameraPosition + myCameraFront, myCameraUp);

		// Projection Matrix
		Matrix4x4 tempProjectionMatrix;
		tempProjectionMatrix = Matrix4x4::Perspective(Math::ToRadians(myFOV), GetAspectRatio(), 0.1f, 100.0f);

		myLightingShader->SetUniformMatrix4x4("ModelMatrix", tempModelMatrix);
		myLightingShader->SetUniformMatrix4x4("ViewMatrix", tempViewMatrix);
		myLightingShader->SetUniformMatrix4x4("ProjectionMatrix", tempProjectionMatrix);

		aModel.Render(*myLightingShader);

		// UI Overlay
		glDisable(GL_DEPTH_TEST);

		// Font rendering
		myImage->Position() = { 50, 50 };
		myImage->Render(*this);

		myText->Text() = std::to_string(GetFPS()) + " FPS";
		myText->Render(*this);

		glEnable(GL_DEPTH_TEST);
	}

private:
	Vector2D myLastMousePosition;

	Vector3D myCameraPosition = { 0.0f, 0.0f, 3.0f };
	Vector3D myCameraUp = { 0.0f, 1.0f, 0.0f };
	Vector3D myCameraFront = { 0.0f, 0.0f, -1.0f };

	float myPitch;
	float myYaw = -90.0f;
	float myRoll;
	float myFOV;

	UIImage* myImage;

	UIText* myText;

	Shader* myLightingShader;
	Shader* myLightCubeShader;
	Shader* myUIImageShader;

	Texture* myTexture;

	Vector3D myLightPosition = { 1.2f, 1.0f, 2.0f };
	Model aModel;
};

Window* BuildWindow()
{
	return new Sandbox("Oxcart", 1920, 1080);
}