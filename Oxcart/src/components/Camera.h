#ifndef CAMERA_HEADER
#define CAMERA_HEADER

#include "Component.h"
#include "Transform.h"

class Camera : public Component
{
public:
	Camera();

	inline Transform& GetTransform() { return myTransform; }
	inline float& Pitch() { return myPitch; }
	inline float& Yaw() { return myYaw; }
	inline float& Roll() { return myRoll; }

private:
	Transform myTransform;
	float myPitch;
	float myYaw;
	float myRoll;
};
#endif