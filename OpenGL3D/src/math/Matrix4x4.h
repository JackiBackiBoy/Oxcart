#ifndef MATRIX_4X4_HEADER
#define MATRIX_4X4_HEADER

#include "Vector3D.h"
#include <array>

struct Matrix4x4
{
	Matrix4x4() {};
	Matrix4x4(const float& aValue);

	float matrixData[4][4] = { 0 };

	static Matrix4x4 LookAt(const Vector3D& aPosition, const Vector3D& aTarget, const Vector3D& anUp);
};
#endif