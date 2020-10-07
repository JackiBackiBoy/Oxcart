#ifndef UI_ELEMENT_HEADER
#define UI_ELEMENT_HEADER

#include "core/Window.h"

class UIElement
{
public:
	UIElement() {};
	virtual ~UIElement();

	virtual void Render(Window& aWindow) = 0;
};
#endif