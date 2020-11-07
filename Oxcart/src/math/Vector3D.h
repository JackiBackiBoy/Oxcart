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

	static const Vector3D Up;
	static const Vector3D Forward;

	// Vector functions
	inline static Vector3D Normalize(const Vector3D& aVector)
	{
		float tempLength = std::sqrt(aVector.x * aVector.x + aVector.y * aVector.y + aVector.z * aVector.z);
		return { aVector.x / tempLength, aVector.y / tempLength, aVector.z / tempLength };
	}

	inline static float DotProduct(const Vector3D& aVector1, const Vector3D& aVector2)
	{
		return aVector1.x * aVector2.x + aVector1.y * aVector2.y + aVector1.z * aVector2.z;
	}

	static Vector3D CrossProduct(const Vector3D& aVector1, const Vector3D& aVector2);

	// Operator overloads
	inline Vector3D operator+(const Vector3D& aVector) const { return { x + aVector.x, y + aVector.y, z + aVector.z }; }

	inline Vector3D& operator+=(const Vector3D& aVector) { x += aVector.x; y += aVector.y; z += aVector.z; return *this; }
	inline Vector3D& operator-=(const Vector3D& aVector) { x -= aVector.x; y -= aVector.y; z -= aVector.z; return *this; }

	inline Vector3D operator-(const Vector3D& aVector) const { return { x - aVector.x, y - aVector.y, z - aVector.z }; }

	inline Vector3D operator*(const Vector3D& aVector) const { return { x * aVector.x, y * aVector.y, z * aVector.z }; }
	inline Vector3D operator*(const float& aFloat) const { return { x * aFloat, y * aFloat, z * aFloat }; }
};
#endif