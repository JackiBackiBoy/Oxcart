#include "Transform.h"

Transform::Transform()
{

}

void Transform::LookAt(Transform& aTarget)
{
	Vector3D tempLookAtDirection = myPosition - aTarget.Position();
	myRotation = Vector3D::Normalize(tempLookAtDirection);

	/*Vector3D tempNormalizedTarget = Vector3D::Normalize(aTarget.Position());

	float tempPitch = atanf(tempNormalizedTarget.y / tempNormalizedTarget.x);
	float tempYaw = atanf(tempNormalizedTarget.y / tempNormalizedTarget.z);*/
}