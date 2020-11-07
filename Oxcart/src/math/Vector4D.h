#ifndef VECTOR_4D_HEADER
#define VECTOR_4D_HEADER

struct Vector4D
{
public:
	Vector4D() : x(0.0f), y(0.0f), z(0.0f), w(0.0f) {};
	Vector4D(const float& anX, const float& aY, const float& aZ, const float& aW) : x(anX), y(aY), z(aZ), w(aW) {};

	float x;
	float y;
	float z;
	float w;

	// Operator overloads
	inline Vector4D operator+(const Vector4D& aVector) { return { x + aVector.x, y + aVector.y, z + aVector.z, w + aVector.w }; }

	inline Vector4D operator*(const float& aFloat) const { return { x * aFloat, y * aFloat, z * aFloat, w * aFloat }; }
};
#endif