#include "UIButton.h"

UIButton::UIButton(const std::string& aText, const Vector2D& aPosition, const int& aWidth, const int& aHeight, const Color& aTextColor, const Color& aButtonColor)
	: myWidth(aWidth), myHeight(aHeight), UIElement(aPosition)
{
	myText = UIText(aText, aPosition, aTextColor);
	myButtonBackground = UIImage(Texture("res/textures/pixel.png"), aPosition, { (float)myWidth, (float)myHeight }, aButtonColor);
	myButtonBackground.myShader = Shader("res/shaders/UIImageShader.glsl");
}

void UIButton::Render(Window& aWindow)
{
	myButtonBackground.Render(aWindow);
	myText.Render(aWindow);
}
