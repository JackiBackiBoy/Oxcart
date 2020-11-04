#ifndef GAME_OBJECT_HEADER
#define GAME_OBJECT_HEADER

#include "math/Vector3D.h"

class GameObject
{
public:
	GameObject(const Vector3D& aPosition) : myPosition(aPosition) {};

	inline Vector3D& Position() { return myPosition; }
	inline Vector3D& Rotation() { return myRotation; }

	virtual void OnUpdate(const float& aDeltaTime);
	virtual void OnRender(const float& aDeltaTime);

protected:
	Vector3D myPosition;
	Vector3D myRotation;
};
#endif