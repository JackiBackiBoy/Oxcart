#ifndef UI_ELEMENT_HEADER
#define UI_ELEMENT_HEADER

class UIElement
{
public:
	UIElement() {};
	virtual ~UIElement();

	virtual void Render() = 0;
};
#endif