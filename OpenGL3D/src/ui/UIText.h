#ifndef UI_TEXT_HEADER
#define UI_TEXT_HEADER

#include <string>
#include <map>
#include "math/Vector2D.h"
#include "Color.h"
#include "UIElement.h"
#include "Character.h"
#include "graphics/Shader.h"

class UIText : public UIElement
{
public:
	UIText(const std::string& someText, const Vector2D& aPosition, const Color& aColor);

	void Render(Window& aWindow) override;

private:
	std::string myText;
	Vector2D myPosition;
	Color myColor;

	std::map<char, Character> myCharacters;
	int myLargestCharacterSize;

	Shader myGlyphShader;

	unsigned int myVAO;
	unsigned int myVBO;
};
#endif