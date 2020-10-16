#include "UIButton.h"

UIButton::UIButton(const std::string& aText, const Vector2D& aPosition, const int& aWidth, const int& aHeight, const Color& aTextColor, const Color& aButtonColor)
	: myWidth(aWidth), myHeight(aHeight), UIElement(aPosition)
{
	myButtonBackground = UIImage(Texture("res/textures/pixel.png"), aPosition, { (float)myWidth, (float)myHeight }, aButtonColor);
	myButtonBackground.myShader = Shader("res/shaders/UIImageShader.glsl");
	myText = UIText(aText, aPosition, aTextColor);
	myText.Position() = { aPosition.x + (myButtonBackground.GetWidth() / 2), 0 };
}

void UIButton::Render(Window& aWindow)
{
	myButtonBackground.Render(aWindow);

	myText.Position() = { myPosition.x + (float)myWidth / 2 - myText.GetTextWidth() / 2, myPosition.y + (float)myHeight / 2 - myText.GetTextHeight() / 2 };
	myText.Render(aWindow);
}