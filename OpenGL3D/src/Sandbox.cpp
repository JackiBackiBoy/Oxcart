#include "core/Window.h"
#include "components/Camera.h"
#include "math/Math.h"

#include "input/Keyboard.h"

class Sandbox : public Window
{
public: 
	Sandbox(const std::string& aTitle, const int& aScreenWidth, const int& aScreenHeight)
		: Window::Window(aTitle, aScreenWidth, aScreenHeight) {}

	void OnStart() override
	{
		myCamera = new Camera();
		myCamera->GetTransform().Position() = { 0.0f, 0.0f, 3.0f };
		myCamera->GetTransform().Rotation() = { 0.0f, 0.0f, -1.0f };


	}

	void OnUpdate(float aDeltaTime) override
	{
		// Forward
		if (Keyboard::IsKeyDown(KeyCode::W, myRawWindow))
		{
			myCameraPosition += 2.0f * aDeltaTime * myCameraFront;
		}
		// Backward
		if (Keyboard::IsKeyDown(KeyCode::S, myRawWindow))
		{
			myCameraPosition -= 2.0f * aDeltaTime * myCameraFront;
		}

		// Left
		if (Keyboard::IsKeyDown(KeyCode::A, myRawWindow))
		{
			myCameraPosition -= Vector3D::Normalize(Vector3D::Cross(myCameraFront, myCameraUp)) * 2.0f * tempDeltaTime;
		}
		// Right
		if (Keyboard::IsKeyDown(KeyCode::D, myRawWindow))
		{
			myCameraPosition += glm::normalize(glm::cross(myCameraFront, myCameraUp)) * 2.0f * tempDeltaTime;
		}

		// Up
		if (Keyboard::IsKeyDown(KeyCode::Space, myRawWindow))
		{
			myCameraPosition.y += 2.0f * tempDeltaTime;
		}
		// Down
		if (Keyboard::IsKeyDown(KeyCode::LeftControl, window))
		{
			myCameraPosition.y -= 2.0f * tempDeltaTime;
		}

		glm::vec2 tempCurrentMousePosition = Mouse::GetPosition(window);

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
		tempDirection.x = cos(Math::ToRadians(myCamera->Yaw())) * cos(Math::ToRadians(myCamera->Pitch()));
		tempDirection.y = sin(Math::ToRadians(myCamera->Pitch()));
		tempDirection.z = sin(Math::ToRadians(myCamera->Yaw())) * cos(Math::ToRadians(myCamera->Pitch()));
		//myCameraFront =
		myCamera->GetTransform().Rotation() = Vector3D::Normalize(tempDirection);
	}

	void OnRender(float aDeltaTime) override
	{

	}

private:
	Camera* myCamera;
};

Window* BuildWindow()
{
	return new Sandbox("Oxcart 3D | Made by Jack Henrikson", 1920, 1080);
}