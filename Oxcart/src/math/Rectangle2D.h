#ifndef RECTANGLE_2D_HEADER
#define RECTANGLE_2D_HEADER

#include "Vector2D.h"

class OX_API Rectangle2D
{
public:
	Rectangle2D() : myPosition({ 0, 0 }), myWidth(0), myHeight(0) {};
	Rectangle2D(const Vector2D& aPosition, const int& aWidth, const int& aHeight);
	~Rectangle2D();

	bool Intersects(const Rectangle2D& aRectangle);
	bool Contains(const Vector2D& aPoint);

	inline int GetWidth() const { return myWidth; }
	inline int GetHeight() const { return myHeight; }
	inline Vector2D& Position() { return myPosition; }

private:
	Vector2D myPosition;
	int myWidth;
	int myHeight;
};
#endif