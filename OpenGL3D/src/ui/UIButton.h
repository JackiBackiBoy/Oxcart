#ifndef UI_BUTTON_HEADER
#define UI_BUTTON_HEADER

#include "UIText.h"
#include "UIImage.h"
#include "Color.h"
#include "math/Rectangle2D.h"

class UIButton : public UIElement
{
public:
	UIButton() : myRectangle({ { 0, 0 }, 0, 0 }) {};
	UIButton(const std::string& aText, const Vector2D& aPosition, const int& aWidth, const int& aHeight, const Color& aTextColor, const Color& aButtonColor);

	void Render(Window& aWindow) override;

	Shader myHoverShader;

private:
	UIText myText;
	UIImage myButtonBackground;
	Rectangle2D myRectangle;
	bool myIsHovered;

	unsigned int myVAO;
	unsigned int myVBO;
	unsigned int myEBO;

	std::array<float, 8> myVertices;
	std::array<unsigned int, 6> myIndices;

	void(*myOnClick)();
};
#endif