#include "UIImage.h"

UIImage::UIImage(const Texture& aTexture, const Vector2D& aPosition, const Vector2D& aScale, const Color& aColor)
	: myTexture(aTexture), myScale(aScale), myColor(aColor), myVAO(0), myVBO(0), myEBO(0), UIElement(aPosition)
{
	myVertices =
	{
		myPosition.x, myPosition.y, 0.0f, 1.0f, // top left
	    myPosition.x, (float)myTexture.GetHeight(), 0.0, 0.0f, // bottom left
		(float)myTexture.GetWidth(), (float)myTexture.GetHeight(), 1.0, 0.0f, // bottom right
		(float)myTexture.GetWidth(), myPosition.x, 1.0f, 1.0f, // top right
	};

	myIndices = { 0, 1, 2, 0, 2, 3 };

	glGenVertexArrays(1, &myVAO);
	glGenBuffers(1, &myVBO);
	glGenBuffers(1, &myEBO);

	// Element Buffer Object (EBO)
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, myEBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(unsigned int), &myIndices[0], GL_STATIC_DRAW);

	// Vertex Array Object (VAO)
	glBindVertexArray(myVAO);
	glBindBuffer(GL_VERTEX_ARRAY, myVAO);

	// Vertex Buffer Object (VBO)
	glBindBuffer(GL_ARRAY_BUFFER, myVBO);
	glBufferData(GL_ARRAY_BUFFER, 16 * sizeof(float), &myVertices[0], GL_STATIC_DRAW);

	// Position and texture coordinates
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, false, sizeof(float) * 4, 0);
}

void UIImage::Render(Window& aWindow)
{
	// Update the vertices in case the image has either been moved or resized
	myVertices[0] = myPosition.x;
	myVertices[1] = myPosition.y;

	myVertices[4] = myPosition.x;
	myVertices[5] = myPosition.y + (float)myTexture.GetHeight() * myScale.y;

	myVertices[8] = myPosition.x + (float)myTexture.GetWidth() * myScale.x;
	myVertices[9] = myPosition.y + (float)myTexture.GetHeight() * myScale.y;

	myVertices[12] = myPosition.x + (float)myTexture.GetWidth() * myScale.x;
	myVertices[13] = myPosition.y;

	// Binding
	glBindVertexArray(myVAO);
	glBindBuffer(GL_ARRAY_BUFFER, myVBO);
	glBufferData(GL_ARRAY_BUFFER, 16 * sizeof(float), &myVertices[0], GL_STATIC_DRAW);

	myShader.Use();

	Matrix4x4 tempProjectionMatrix = Matrix4x4::Ortographic(0, aWindow.GetScreenWidth(), aWindow.GetScreenHeight(), 0);
	myShader.SetUniformMatrix4x4("ProjectionMatrix", tempProjectionMatrix);

	Vector4D tempNormalizedColor = Color::Normalize(myColor);
	myShader.SetUniform4f("ColorOverlay", tempNormalizedColor.x, tempNormalizedColor.y, tempNormalizedColor.z, tempNormalizedColor.w);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, myTexture.GetID());

	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, &myIndices[0]);

	glBindVertexArray(0);
	glActiveTexture(GL_TEXTURE0);
}