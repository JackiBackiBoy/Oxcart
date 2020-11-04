#include "Bird.h"
#include "input/Keyboard.h"
#include "input/Mouse.h"

void Bird::OnUpdate(const float& aDeltaTime)
{
	myPosition += { myVelocity.x * aDeltaTime, myVelocity.y * aDeltaTime, 0 };
	myVelocity.y -= myGravityForce;
	
	// Update hitbox
	//myHitbox = { }

	// Wing rotation when flapping
	if (myWingRotation.x > -100.0f)
	{
		myWingRotation.x -= 250.0f * aDeltaTime;
	}

	if (Keyboard::IsKeyDown(KeyCode::Space, Window::CurrentWindow->GetRawWindow()) || Mouse::IsButtonDown(MouseButton::Left, Window::CurrentWindow->GetRawWindow()))
	{
		if (myAllowJump)
		{
			myWingRotation.x = -1.0f;
			myVelocity.y = myJumpForce;
			myAllowJump = false;
		}
	}
	else
	{
		myAllowJump = true;
	}

	// Calculate rotation of bird depending on current velocity
	myRotation.z = (myVelocity.y / myJumpForce) * 30;
}