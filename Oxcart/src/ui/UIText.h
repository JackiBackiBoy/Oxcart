#ifndef UI_TEXT_HEADER
#define UI_TEXT_HEADER

#include <string>
#include <map>
#include "math/Vector3D.h"
#include "Color.h"
#include "UIElement.h"
#include "Character.h"
#include "graphics/Shader.h"

class OX_API UIText : public UIElement
{
public:
	UIText() : myText(""), myColor{ 0, 0, 0 }, myScale({ 1, 1 }) {};
	UIText(const std::string& someText, const Vector2D& aPosition, const Color& aColor);

	inline std::string& Text() { return myText; }
	inline Color& AccessColor() { return myColor; }

	int GetTextWidth();
	int GetTextHeight();

	Vector2D& Scale();

	void Render(Window& aWindow) override;

private:
	std::string myText;
	Color myColor;

	std::map<char, Character> myCharacters;
	int myLargestCharacterSize;
	int myTextWidth;
	int myTextHeight;
	Vector2D myScale;

	Shader myGlyphShader;

	unsigned int myVAO;
	unsigned int myVBO;
};
#endif