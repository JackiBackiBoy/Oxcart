#ifndef UI_ELEMENT_HEADER
#define UI_ELEMENT_HEADER

#include "core/Window.h"
#include "math/Vector3D.h"

class UIElement
{
public:
	UIElement() : myPosition({ 0.0f, 0.0f, 0.0f }) {};
	UIElement(const Vector3D& aPosition) : myPosition(aPosition) {};
	virtual ~UIElement();

	virtual void Render(Window& aWindow) = 0;

	inline Vector3D& Position() { return myPosition; }

protected:
	Vector3D myPosition;
};
#endif