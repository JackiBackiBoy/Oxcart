#include <iostream>
#include "GameInfo.h"
#include "GameActions.h"
#include "gameobjects/Bird.h"
#include "gameobjects/Pipe.h"
#include "gameobjects/PipeManager.h"

#include "Oxcart.h"

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
		tempHoverShader = new Shader("res/shaders/HoverShader.glsl");
		
		myText = new UIText("Beep beep boop", { 10, 10 }, { 255, 255, 255 });
		myFOV = 70.0f;
		
		myTexture = new Texture("res/textures/wood_box.png");
		myImage = new UIImage(*myTexture, { 0, 0 });
		myImage->myShader = *myUIImageShader;

		myFileButton = UIButton("Arkiv", { 0, 0 }, 100, 9, { 0, 0, 0 }, { 255, 255, 255 });
		myFileButton.myHoverShader = *tempHoverShader;
		myFileButton.SetOnClick(GameActions::ContinueGame);

		myButton = new UIButton("Play", { 100, 100 }, 300, 100, { 255, 255, 255 }, { 120, 100, 0 });
		myButton->myHoverShader = *tempHoverShader;
		myButton->SetOnClick(GameActions::ContinueGame);

		//glEnable(GL_DEPTH_TEST);
		//glEnable(GL_BLEND);
		SetVSync(false);
		//glfwSetInputMode(myRawWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

		aModel = Model("res/models/Flappy Bird/untitled.obj");
		myWing = Model("res/models/Flappy Bird Wing/flappy_bird_wing.obj");
		myPipe = Model("res/models/Pipe/pipe.obj");
		myNanoSuit = Model("res/models/Nanosuit/nanosuit.obj");

		PipeManager::PipeModel = myPipe;

		myBird = Bird({ 0, 10, 0 }, 30.0f, 0.1f, { { 0, 10, }, 1, 1 });
	}

	void OnUpdate(const float& aDeltaTime) override
	{
		if (Keyboard::IsKeyDownOnce(KeyCode::Escape, myRawWindow))
		{
			if (GameInfo::myCurrentGameState == GameState::Playing)
			{
				GameInfo::myCurrentGameState = GameState::Paused;
			}
			else if (GameInfo::myCurrentGameState == GameState::Paused)
			{
				GameInfo::myCurrentGameState = GameState::Playing;
			}
		}

		if (GameInfo::myCurrentGameState == GameState::Playing)
		{
			HideMouse();

			myFOV += -Mouse::GetScrollWheel() * 2.0f;

			myCameraPosition.y = Math::Lerp(myCameraPosition.y, myBird.Position().y, 0.5f) + 6;

			myBird.OnUpdate(aDeltaTime);
			PipeManager::Update(aDeltaTime);

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
				myPitch = -89.0f;
			}

			Vector3D tempDirection;
			tempDirection.x = cos(Math::ToRadians(myYaw)) * cos(Math::ToRadians(myPitch));
			tempDirection.y = sin(Math::ToRadians(myPitch));
			tempDirection.z = sin(Math::ToRadians(myYaw)) * cos(Math::ToRadians(myPitch));
			myCameraFront = Vector3D::Normalize(tempDirection);
		}
		else if (GameInfo::myCurrentGameState == GameState::Paused)
		{
			ShowMouse();
		}
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
		tempModelMatrix = Matrix4x4::Translate(tempModelMatrix, myBird.Position());
		tempModelMatrix = Matrix4x4::Rotate(tempModelMatrix, Math::ToRadians(myBird.Rotation().z), { 0.0f, 0.0, 1.0f });

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

		// Wing
		tempModelMatrix = Matrix4x4::Identity();
		tempModelMatrix = Matrix4x4::Translate(tempModelMatrix, { myBird.Position().x - 1.02f, myBird.Position().y - 0.2f, myBird.Position().z + 0.49f });
		tempModelMatrix = Matrix4x4::Rotate(tempModelMatrix, Math::ToRadians(myBird.Rotation().z), { 0.0f, 0.0, 1.0f });
		tempModelMatrix = Matrix4x4::Rotate(tempModelMatrix, Math::ToRadians(myBird.GetWingRotation().x), { 1.0f, 0.0, 0.0f });
		myLightingShader->SetUniformMatrix4x4("ModelMatrix", tempModelMatrix);

		myWing.Render(*myLightingShader);

		// Pipe
		//tempModelMatrix = Matrix4x4::Identity();
		//tempModelMatrix = Matrix4x4::Scale(tempModelMatrix, { 3, 3, 3 });
		//tempModelMatrix = Matrix4x4::Translate(tempModelMatrix, myPipeObject.Position());
		//myLightingShader->SetUniformMatrix4x4("ModelMatrix", tempModelMatrix);

		//myPipe.Render(*myLightingShader);

		PipeManager::Render(*myLightingShader);

		// UI Overlay
		if (GameInfo::myCurrentGameState == GameState::Paused)
		{
			SetDepthTest(false);

			//myButton->Render(*this);
			myFileButton.Render(*this);

			SetDepthTest(true);
		}

		//myNanoSuit.Render(*myLightingShader);
	}

private:
	Vector2D myLastMousePosition;

	Vector3D myCameraPosition = { 10, 10, 13 };
	Vector3D myCameraUp = { 0.0f, 1.0f, 0.0f };
	Vector3D myCameraFront = { -1.0, 0.0f, -1.0f };

	float myPitch = -5;
	float myYaw = -125;
	float myRoll;
	float myFOV;

	UIImage* myImage;

	UIText* myText;
	UIButton* myButton;
	UIButton myFileButton;

	Shader* myLightingShader;
	Shader* myLightCubeShader;
	Shader* myUIImageShader;
	Shader* tempHoverShader;

	Texture* myTexture;
	Vector3D myLightPosition = { 1.2f, 1.0f, 2.0f };
	Model aModel;
	Model myWing;
	Model myPipe;
	Model myNanoSuit;

	Bird myBird;
};

Window* BuildWindow()
{
	return new Sandbox("Oxcart", 1280, 720);
}