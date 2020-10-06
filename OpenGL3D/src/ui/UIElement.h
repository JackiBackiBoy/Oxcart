#ifndef UI_ELEMENT_HEADER
#define UI_ELEMENT_HEADER

#include <string>

class UIElement
{
public:
	UIElement() {};
	~UIElement();

	virtual void Render() = 0;
};
#endif