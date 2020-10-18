#ifndef HITBOX_2D_HEADER
#define HITBOX_2D_HEADER

#include "math/Vector2D.h"

class Hitbox2D
{
public:
	Hitbox2D() {};
	Hitbox2D(const Vector2D& aPosition, const int& aWidth, const int& aHeight);

private:
	int myWidth;
	int myHeight;
};
#endif