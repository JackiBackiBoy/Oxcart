#ifndef COLOR_HEADER
#define COLOR_HEADER

#include "math/Vector4D.h"

struct OX_API Color
{
public:
	Color() : r(0), g(0), b(0), a(0) {};
	Color(const unsigned char& aRed, const unsigned char& aGreen, const unsigned char& aBlue, const unsigned char& anAlpha = 255)
		: r(aRed), g(aGreen), b(aBlue), a(anAlpha) {};

	inline static Vector4D Normalize(const Color& aColor) { return Vector4D((float)aColor.r, (float)aColor.g, (float)aColor.b, (float)aColor.a) * ColorQuotient; }

	unsigned char r;
	unsigned char g;
	unsigned char b;
	unsigned char a;

private:
	static constexpr float ColorQuotient = 1.0f / 255;
};
#endif