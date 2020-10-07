#ifndef UI_TEXT_HEADER
#define UI_TEXT_HEADER

#include <string>
#include "math/Vector2D.h"
#include "Color.h"
#include "UIElement.h"

class UIText : public UIElement
{
public:
	UIText(const std::string& someText, const Vector2D& aPosition, const Color& aColor);

	void Render() override;

private:
	std::string myText;
};
#endif