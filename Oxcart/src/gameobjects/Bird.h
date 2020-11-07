#ifndef BIRD_HEADER
#define BIRD_HEADER

#include "GameObject.h"
#include "math/Rectangle2D.h"

class Bird : public GameObject
{
public:
	Bird() : myJumpForce(0), myGravityForce(0), myAllowJump(true), myHitbox({ 0, 0 }, 0, 0), myWingRotation(0, 0, 0), GameObject({ 0, 0, 0 }) {};
	Bird(const Vector3D& aPosition, const float& aJumpForce, const float& aGravityForce, const Rectangle2D& aHitbox)
		: myJumpForce(aJumpForce), myGravityForce(aGravityForce), myAllowJump(true), myHitbox(aHitbox), GameObject(aPosition) {};

	void OnUpdate(const float& aDeltaTime) override;

	inline float& JumpForce() { return myJumpForce; }
	inline float& GravityForce() { return myGravityForce; }
	inline Vector3D GetWingRotation() { return myWingRotation; }

private:
	Vector3D myVelocity;
	float myJumpForce;
	float myGravityForce;
	bool myAllowJump;
	Rectangle2D myHitbox;

	Vector3D myWingRotation;

};
#endif