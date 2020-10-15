#ifndef UI_BUTTON_HEADER
#define UI_BUTTON_HEADER

#include "UIText.h"
#include "UIImage.h"
#include "Color.h"

class UIButton : public UIElement
{
public:
	UIButton() : myWidth(0), myHeight(0) {};
	UIButton(const std::string& aText, const Vector2D& aPosition, const int& aWidth, const int& aHeight, const Color& aTextColor, const Color& aButtonColor);

	void Render(Window& aWindow) override;

private:
	UIText myText;
	UIImage myButtonBackground;
	int myWidth;
	int myHeight;
};
#endif