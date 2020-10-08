#ifndef UI_IMAGE_HEADER
#define UI_IMAGE_HEADER

#include "UIElement.h"
#include "graphics/Texture.h"
#include "graphics/Shader.h"

class UIImage : public UIElement
{
public:
	UIImage();
	UIImage(const Texture& aTexture, const Vector3D& aPosition) : myTexture(aTexture), UIElement(aPosition) {};

	void Render(Window& aWindow) override;

	inline Texture& AccessTexture() { return myTexture; }

	Shader myShader;

private:
	Texture myTexture;
	unsigned int myVAO;
	unsigned int myVBO;
};
#endif