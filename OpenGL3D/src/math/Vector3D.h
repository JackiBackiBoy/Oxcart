#ifndef VECTOR_3D_HEADER
#define VECTOR_3D_HEADER

#include <cmath>

struct Vector3D
{
	Vector3D() : x(0.0f), y(0.0f), z(0.0f) {};
	Vector3D(const float& anX, const float& aY, const float& aZ) : x(anX), y(aY), z(aZ) {};

	float x;
	float y;
	float z;

	inline static Vector3D Forward() { return { 0, 0, -1 }; }

	// Operator overloads
	inline Vector3D operator+(const Vector3D& aVector) const { return { x + aVector.x, y + aVector.y, z + aVector.z }; }
	inline Vector3D operator-(const Vector3D& aVector) const { return { x - aVector.x, y - aVector.y, z - aVector.z }; }
	inline Vector3D operator*(const Vector3D& aVector) const { return { x * aVector.x, y * aVector.y, z * aVector.z }; }

	// Vector functions
	inline static Vector3D Normalize(const Vector3D& aVector)
	{
		float tempLength = std::sqrt(aVector.x * aVector.x + aVector.y * aVector.y + aVector.z * aVector.z);
		return { aVector.x / tempLength, aVector.y / tempLength, aVector.z / tempLength };
	}
	static Vector3D Cross(const Vector3D& aVector1, const Vector3D& aVector2);
};
#endif