#ifndef VECTOR_4D_HEADER
#define VECTOR_4D_HEADER

struct Vector4D
{
public:
	float x = 0.0f;
	float y = 0.0f;
	float z = 0.0f;
	float w = 1.0f;

	// Operator overloads
	inline Vector4D operator+(const Vector4D& aVector) { return { x + aVector.x, y + aVector.y, z + aVector.z, w + aVector.w }; }

	inline Vector4D operator*(const float& aFloat) const { return { x * aFloat, y * aFloat, z * aFloat, w * aFloat }; }
};
#endif