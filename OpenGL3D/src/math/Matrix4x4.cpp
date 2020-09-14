#include "Matrix4x4.h"
#include <algorithm>

Matrix4x4::Matrix4x4(const float& aValue)
{
	std::fill(&matrixData[0][0], &matrixData[0][0] + 16, aValue);
}

Matrix4x4 Matrix4x4::LookAt(const Vector3D& aPosition, const Vector3D& aTarget, const Vector3D& anUp)
{
	Vector3D tempForward = Vector3D::Normalize(aPosition - aTarget);
	Vector3D tempRight = Vector3D::CrossProduct(Vector3D::Normalize({ 0, 1, 0 }), tempForward);
	Vector3D tempUp = Vector3D::CrossProduct(tempForward, tempRight);

	Matrix4x4 tempLookAtMatrix;
	tempLookAtMatrix.matrixData[0][0] = tempRight.x;
	tempLookAtMatrix.matrixData[0][1] = tempRight.y;
	tempLookAtMatrix.matrixData[0][2] = tempRight.z;
	tempLookAtMatrix.matrixData[0][3] = 0.0f;

	tempLookAtMatrix.matrixData[1][0] = tempUp.x;
	tempLookAtMatrix.matrixData[1][1] = tempUp.y;
	tempLookAtMatrix.matrixData[1][2] = tempUp.z;
	tempLookAtMatrix.matrixData[1][3] = 0.0f;

	tempLookAtMatrix.matrixData[2][0] = tempForward.x;
	tempLookAtMatrix.matrixData[2][1] = tempForward.y;
	tempLookAtMatrix.matrixData[2][2] = tempForward.z;
	tempLookAtMatrix.matrixData[2][3] = 0.0f;

	tempLookAtMatrix.matrixData[3][0] = aPosition.x;
	tempLookAtMatrix.matrixData[3][1] = aPosition.y;
	tempLookAtMatrix.matrixData[3][2] = aPosition.z;
	tempLookAtMatrix.matrixData[3][3] = 1.0f;

	return tempLookAtMatrix;
}