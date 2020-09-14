#include "Vector3D.h"

const Vector3D Vector3D::Up = { 0, 1, 0 };
const Vector3D Vector3D::Forward = { 0, 0, -1 };

Vector3D Vector3D::CrossProduct(const Vector3D& aVector1, const Vector3D& aVector2)
{
	Vector3D tempCrossProduct;
	tempCrossProduct.x = aVector1.y * aVector2.z - aVector1.z * aVector2.y;
	tempCrossProduct.y = aVector1.z * aVector2.x - aVector1.x * aVector2.z;
	tempCrossProduct.z = aVector1.x * aVector2.y - aVector1.y * aVector2.x;

	return tempCrossProduct;
}