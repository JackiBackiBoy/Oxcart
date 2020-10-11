#ifndef UI_IMAGE_HEADER
#define UI_IMAGE_HEADER

#include "UIElement.h"
#include "graphics/Texture.h"
#include "graphics/Shader.h"
#include "math/Vector2D.h"
#include <array>

class UIImage : public UIElement
{
public:
	UIImage() : myVAO(0), myVBO(0), myEBO(0), myScale({ 1, 1 }), myVertices({ 0.0f }), myIndices({ 0 }), UIElement({ 0, 0 }) {};
	UIImage(const Texture& aTexture, const Vector2D& aPosition, const Vector2D& aScale = { 1, 1 });

	void Render(Window& aWindow) override;

	inline Texture& AccessTexture() { return myTexture; }
	inline Vector2D& Scale() { return myScale; }

	Shader myShader;

private:
	Texture myTexture;
	unsigned int myVAO;
	unsigned int myVBO;
	unsigned int myEBO;

	Vector2D myScale;
	std::array<float, 16> myVertices;
	std::array<unsigned int, 6> myIndices;
};
#endif