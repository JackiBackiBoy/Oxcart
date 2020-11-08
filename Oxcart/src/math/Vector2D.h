#ifndef VECTOR_2D_HEADER
#define VECTOR_2D_HEADER

#include "core/Core.h"
#include <cmath>

struct OX_API Vector2D
{
	Vector2D() : x(0.0f), y(0.0f) {};
	Vector2D(const float& anX, const float& aY) : x(anX), y(aY) {};

	float x;
	float y;

	// Operator overloads
	inline Vector2D operator+(const Vector2D& aVector) const { return { x + aVector.x, y + aVector.y }; }
	inline Vector2D operator-(const Vector2D& aVector) const { return { x - aVector.x, y - aVector.y }; }
	inline Vector2D operator*(const Vector2D& aVector) const { return { x * aVector.x, y * aVector.y }; }

	// Vector functions
	inline static Vector2D Normalize(const Vector2D& aVector)
	{
		float tempLength = std::sqrt(aVector.x * aVector.x + aVector.y * aVector.y);
		return { aVector.x / tempLength, aVector.y / tempLength };
	}
};
#endif