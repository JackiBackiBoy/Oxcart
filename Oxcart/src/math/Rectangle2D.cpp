#include "Rectangle2D.h"

Rectangle2D::Rectangle2D(const Vector2D& aPosition, const int& aWidth, const int& aHeight)
	: myPosition(aPosition), myWidth(aWidth), myHeight(aHeight)
{

}

Rectangle2D::~Rectangle2D()
{

}

bool Rectangle2D::Intersects(const Rectangle2D& aRectangle)
{
	return true;
}

bool Rectangle2D::Contains(const Vector2D& aPoint)
{
	if (aPoint.x >= myPosition.x && aPoint.x < myPosition.x + myWidth && aPoint.y >= myPosition.y && aPoint.y < myPosition.y + myHeight)
	{
		return true;
	}

	return false;
}

int Rectangle2D::GetWidth()
{
	return myWidth;
}

int Rectangle2D::GetHeight()
{
	return myHeight;
}
