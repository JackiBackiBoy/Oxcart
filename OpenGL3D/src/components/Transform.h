#ifndef TRANSFORM_HEADER
#define TRANSFORM_HEADER

#include "Component.h"
#include "math/Vector3D.h"

class Transform : public Component
{
public:
	Transform();

	inline Vector3D& Position() { return myPosition; }
	inline Vector3D& Rotation() { return myRotation; }
	inline Vector3D GetForward() const { return myRotation * Vector3D::Forward(); }
	//inline void SetForward(const Vector3D& aForward) { myRotation = LookAt(aForward); }

	// Functions
	void LookAt(Transform& aTarget);

private:
	Vector3D myPosition;
	Vector3D myRotation;
};
#endif