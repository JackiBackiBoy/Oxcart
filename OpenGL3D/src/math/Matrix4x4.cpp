#include "Matrix4x4.h"
#include "Vector4D.h"
#include <algorithm>

Matrix4x4 Matrix4x4::Identity()
{
	Matrix4x4 tempIdentityMatrix;

	tempIdentityMatrix.matrixData[0][0] = 1;
	tempIdentityMatrix.matrixData[1][1] = 1;
	tempIdentityMatrix.matrixData[2][2] = 1;
	tempIdentityMatrix.matrixData[3][3] = 1;

	return tempIdentityMatrix;
}

Matrix4x4 Matrix4x4::LookAt(const Vector3D& aPosition, const Vector3D& aTarget, const Vector3D& anUp)
{
	Vector3D tempForward = Vector3D::Normalize(aTarget - aPosition);
	Vector3D tempRight = Vector3D::Normalize(Vector3D::CrossProduct(tempForward, anUp));
	Vector3D tempUp = Vector3D::CrossProduct(tempRight, tempForward);

	Matrix4x4 tempLookAtMatrix = Matrix4x4::Identity();

	tempLookAtMatrix.matrixData[0][0] = tempRight.x;
	tempLookAtMatrix.matrixData[1][0] = tempRight.y;
	tempLookAtMatrix.matrixData[2][0] = tempRight.z;

	tempLookAtMatrix.matrixData[0][1] = tempUp.x;
	tempLookAtMatrix.matrixData[1][1] = tempUp.y;
	tempLookAtMatrix.matrixData[2][1] = tempUp.z;

	tempLookAtMatrix.matrixData[0][2] = -tempForward.x;
	tempLookAtMatrix.matrixData[1][2] = -tempForward.y;
	tempLookAtMatrix.matrixData[2][2] = -tempForward.z;

	tempLookAtMatrix.matrixData[3][0] = -Vector3D::DotProduct(tempRight, aPosition);
	tempLookAtMatrix.matrixData[3][1] = -Vector3D::DotProduct(tempUp, aPosition);
	tempLookAtMatrix.matrixData[3][2] = Vector3D::DotProduct(tempForward, aPosition);

	return tempLookAtMatrix;
}

Matrix4x4 Matrix4x4::Rotate(const Matrix4x4& aMatrix, const float& someRadians, const Vector3D& anAxis)
{
	float tempCosineAngle = cosf(someRadians);
	float tempSineAngle = sinf(someRadians);

	Vector3D tempNormalizedAxis = Vector3D::Normalize(anAxis);
	Vector3D tempFactor = tempNormalizedAxis * (1.0f - tempCosineAngle); // factor for each axis used in the matrix, compute once for better performance

	// Rodriguez Rotation Formula
	Matrix4x4 tempRotation;

	tempRotation.matrixData[0][0] = tempCosineAngle + tempFactor.x * tempNormalizedAxis.x;
	tempRotation.matrixData[0][1] = tempFactor.x * tempNormalizedAxis.y + tempSineAngle * tempNormalizedAxis.z;
	tempRotation.matrixData[0][2] = tempFactor.x * tempNormalizedAxis.z - tempSineAngle * tempNormalizedAxis.y;

	tempRotation.matrixData[1][0] = tempFactor.y * tempNormalizedAxis.x - tempSineAngle * tempNormalizedAxis.z;
	tempRotation.matrixData[1][1] = tempCosineAngle + tempFactor.y * tempNormalizedAxis.y;
	tempRotation.matrixData[1][2] = tempFactor.y * tempNormalizedAxis.z + tempSineAngle * tempNormalizedAxis.x;

	tempRotation.matrixData[2][0] = tempFactor.z * tempNormalizedAxis.x + tempSineAngle * tempNormalizedAxis.y;
	tempRotation.matrixData[2][1] = tempFactor.z * tempNormalizedAxis.y - tempSineAngle * tempNormalizedAxis.x;
	tempRotation.matrixData[2][2] = tempCosineAngle + tempFactor.z * tempNormalizedAxis.z;


	// Result matrix calculation
	Vector4D tempColumn1 = { aMatrix.matrixData[0][0], aMatrix.matrixData[0][1], aMatrix.matrixData[0][2], aMatrix.matrixData[0][3] };
	Vector4D tempColumn2 = { aMatrix.matrixData[1][0], aMatrix.matrixData[1][1], aMatrix.matrixData[1][2], aMatrix.matrixData[1][3] };
	Vector4D tempColumn3 = { aMatrix.matrixData[2][0], aMatrix.matrixData[2][1], aMatrix.matrixData[2][2], aMatrix.matrixData[2][3] };
	Vector4D tempColumn4 = { aMatrix.matrixData[3][0], aMatrix.matrixData[3][1], aMatrix.matrixData[3][2], aMatrix.matrixData[3][3] };

	Matrix4x4 tempResult;

	Vector4D tempColumn1Result = tempColumn1 * tempRotation.matrixData[0][0] + tempColumn2 * tempRotation.matrixData[0][1] + tempColumn3 * tempRotation.matrixData[0][2];
	Vector4D tempColumn2Result = tempColumn1 * tempRotation.matrixData[1][0] + tempColumn2 * tempRotation.matrixData[1][1] + tempColumn3 * tempRotation.matrixData[1][2];
	Vector4D tempColumn3Result = tempColumn1 * tempRotation.matrixData[2][0] + tempColumn2 * tempRotation.matrixData[2][1] + tempColumn3 * tempRotation.matrixData[2][2];
	Vector4D tempColumn4Result = tempColumn4;

	// Column 1
	tempResult.matrixData[0][0] = tempColumn1Result.x;
	tempResult.matrixData[0][1] = tempColumn1Result.y;
	tempResult.matrixData[0][2] = tempColumn1Result.z;
	tempResult.matrixData[0][3] = tempColumn1Result.w;

	// Column 2
	tempResult.matrixData[1][0] = tempColumn2Result.x;
	tempResult.matrixData[1][1] = tempColumn2Result.y;
	tempResult.matrixData[1][2] = tempColumn2Result.z;
	tempResult.matrixData[1][3] = tempColumn2Result.w;

	// Column 3
	tempResult.matrixData[2][0] = tempColumn3Result.x;
	tempResult.matrixData[2][1] = tempColumn3Result.y;
	tempResult.matrixData[2][2] = tempColumn3Result.z;
	tempResult.matrixData[2][3] = tempColumn3Result.w;

	// Column 4
	tempResult.matrixData[3][0] = tempColumn4Result.x;
	tempResult.matrixData[3][1] = tempColumn4Result.y;
	tempResult.matrixData[3][2] = tempColumn4Result.z;
	tempResult.matrixData[3][3] = tempColumn4Result.w;

	return tempResult;
}

Matrix4x4 Matrix4x4::Perspective(const float& someFOV, const float& anAspectRatio, const float& aZNear, const float& aZFar)
{
	float tempTanHalfFOV = tanf(someFOV / 2);

	Matrix4x4 tempMatrix;

	tempMatrix.matrixData[0][0] = 1.0f / (anAspectRatio * tempTanHalfFOV);
	tempMatrix.matrixData[1][1] = 1.0f / tempTanHalfFOV;
	tempMatrix.matrixData[2][2] = -(aZFar + aZNear) / (aZFar - aZNear);
	tempMatrix.matrixData[2][3] = -1;
	tempMatrix.matrixData[3][2] = -(2 * aZFar * aZNear) / (aZFar - aZNear);

	return tempMatrix;
}

Matrix4x4 Matrix4x4::Ortographic(const float& aLeft, const float& aRight, const float& aBottom, const float& aTop)
{
	Matrix4x4 tempResult = Matrix4x4::Identity();
	tempResult.matrixData[0][0] = 2.0f / (aRight - aLeft);
	tempResult.matrixData[1][1] = 2.0f / (aTop - aBottom);
	tempResult.matrixData[2][2] = -1.0f;
	tempResult.matrixData[3][0] = -(aRight + aLeft) / (aRight - aLeft);
	tempResult.matrixData[3][1] = -(aTop + aBottom) / (aTop - aBottom);
	return tempResult;
}

Matrix4x4 Matrix4x4::Translate(const Matrix4x4& aMatrix, const Vector3D& aVector)
{
	Matrix4x4 tempMatrix = aMatrix;

	Vector4D tempColumn1 = { aMatrix.matrixData[0][0], aMatrix.matrixData[0][1], aMatrix.matrixData[0][2], aMatrix.matrixData[0][3] };
	Vector4D tempColumn2 = { aMatrix.matrixData[1][0], aMatrix.matrixData[1][1], aMatrix.matrixData[1][2], aMatrix.matrixData[1][3] };
	Vector4D tempColumn3 = { aMatrix.matrixData[2][0], aMatrix.matrixData[2][1], aMatrix.matrixData[2][2], aMatrix.matrixData[2][3] };
	Vector4D tempColumn4 = { aMatrix.matrixData[3][0], aMatrix.matrixData[3][1], aMatrix.matrixData[3][2], aMatrix.matrixData[3][3] };

	Vector4D tempColumnResult = tempColumn1 * aVector.x + tempColumn2 * aVector.y + tempColumn3 * aVector.z + tempColumn4;

	tempMatrix.matrixData[3][0] = tempColumnResult.x;
	tempMatrix.matrixData[3][1] = tempColumnResult.y;
	tempMatrix.matrixData[3][2] = tempColumnResult.z;
	tempMatrix.matrixData[3][3] = tempColumnResult.w;

	return tempMatrix;
}

Matrix4x4 Matrix4x4::Scale(const Matrix4x4& aMatrix, const Vector3D& aScale)
{
	Matrix4x4 tempResult;

	Vector4D tempColumn1 = { aMatrix.matrixData[0][0], aMatrix.matrixData[0][1], aMatrix.matrixData[0][2], aMatrix.matrixData[0][3] };
	Vector4D tempColumn2 = { aMatrix.matrixData[1][0], aMatrix.matrixData[1][1], aMatrix.matrixData[1][2], aMatrix.matrixData[1][3] };
	Vector4D tempColumn3 = { aMatrix.matrixData[2][0], aMatrix.matrixData[2][1], aMatrix.matrixData[2][2], aMatrix.matrixData[2][3] };
	Vector4D tempColumn4 = { aMatrix.matrixData[3][0], aMatrix.matrixData[3][1], aMatrix.matrixData[3][2], aMatrix.matrixData[3][3] };

	Vector4D tempColumn1Result = tempColumn1 * aScale.x;
	Vector4D tempColumn2Result = tempColumn2 * aScale.y;
	Vector4D tempColumn3Result = tempColumn3 * aScale.z;
	Vector4D tempColumn4Result = tempColumn4;

	// Column 1
	tempResult.matrixData[0][0] = tempColumn1Result.x;
	tempResult.matrixData[0][1] = tempColumn1Result.y;
	tempResult.matrixData[0][2] = tempColumn1Result.z;
	tempResult.matrixData[0][3] = tempColumn1Result.w;

	// Column 2
	tempResult.matrixData[1][0] = tempColumn2Result.x;
	tempResult.matrixData[1][1] = tempColumn2Result.y;
	tempResult.matrixData[1][2] = tempColumn2Result.z;
	tempResult.matrixData[1][3] = tempColumn2Result.w;

	// Column 3
	tempResult.matrixData[2][0] = tempColumn3Result.x;
	tempResult.matrixData[2][1] = tempColumn3Result.y;
	tempResult.matrixData[2][2] = tempColumn3Result.z;
	tempResult.matrixData[2][3] = tempColumn3Result.w;

	// Column 4
	tempResult.matrixData[3][0] = tempColumn4Result.x;
	tempResult.matrixData[3][1] = tempColumn4Result.y;
	tempResult.matrixData[3][2] = tempColumn4Result.z;
	tempResult.matrixData[3][3] = tempColumn4Result.w;

	return tempResult;
}