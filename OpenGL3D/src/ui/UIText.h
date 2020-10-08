#ifndef UI_TEXT_HEADER
#define UI_TEXT_HEADER

#include <string>
#include <map>
#include "math/Vector3D.h"
#include "Color.h"
#include "UIElement.h"
#include "Character.h"
#include "graphics/Shader.h"

class UIText : public UIElement
{
public:
	UIText() {};
	UIText(const std::string& someText, const Vector3D& aPosition, const Color& aColor);

	inline std::string& Text() { return myText; }
	inline Color& AccessColor() { return myColor; }

	void Render(Window& aWindow) override;

private:
	std::string myText;
	Color myColor;

	std::map<char, Character> myCharacters;
	int myLargestCharacterSize;

	Shader myGlyphShader;

	unsigned int myVAO;
	unsigned int myVBO;
};
#endif