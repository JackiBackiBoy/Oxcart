#ifndef UI_ELEMENT_HEADER
#define UI_ELEMENT_HEADER

#include "core/Window.h"
#include "math/Vector2D.h"

class UIElement
{
public:
	UIElement() : myPosition({ 0, 0 }) {};
	UIElement(const Vector2D& aPosition) : myPosition(aPosition) {};
	virtual ~UIElement();

	virtual void Render(Window& aWindow) = 0;

	inline Vector2D& Position() { return myPosition; }

protected:
	Vector2D myPosition;
};
#endif