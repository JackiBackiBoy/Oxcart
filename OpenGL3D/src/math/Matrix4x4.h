#ifndef MATRIX_4X4_HEADER
#define MATRIX_4X4_HEADER

#include "Vector3D.h"
#include <array>

struct Matrix4x4
{
	Matrix4x4() {};

	float matrixData[4][4] = { 0 };

	inline float* GetValuePtr() { return &(matrixData[0][0]); }

	static Matrix4x4 Identity();

	static Matrix4x4 LookAt(const Vector3D& aPosition, const Vector3D& aTarget, const Vector3D& anUp);
	static Matrix4x4 Rotate(const Matrix4x4& aMatrix, const float& someRadians, const Vector3D& anAxis);
	static Matrix4x4 Perspective(const float& someFOV, const float& anAspectRatio, const float& aZNear, const float& aZFar);
	static Matrix4x4 Ortographic(const float& aLeft, const float& aRight, const float& aBottom, const float& aTop);
	static Matrix4x4 Translate(const Matrix4x4& aMatrix, const Vector3D& aVector);
	static Matrix4x4 Scale(const Matrix4x4& aMatrix, const Vector3D& aVectorScalar);
};
#endif