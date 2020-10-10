#include "UIImage.h"

UIImage::UIImage(const Texture& aTexture, const Vector3D& aPosition) : myTexture(aTexture), myVAO(0), myVBO(0), myEBO(0), UIElement(aPosition)
{
	glGenVertexArrays(1, &myVAO);
	glGenBuffers(1, &myVBO);
	glGenBuffers(1, &myEBO);
}

void UIImage::Render(Window& aWindow)
{
	// A Quad
	float tempVertices[4][4] =
	{
		myPosition.x,  myPosition.y, 0.0f, 1.0f, // top left
		myPosition.x, (float)myTexture.GetHeight(), 0.0,  0.0f, // bottom left
		(float)myTexture.GetWidth(), (float)myTexture.GetHeight(), 1.0,  0.0f, // bottom right
		(float)myTexture.GetWidth(), myPosition.x, 1.0f, 1.0f, // top right
	};

	unsigned int tempIndices[6] = { 0, 1, 2, 0, 2, 3 };


	// Element Buffer Object (EBO)
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, myEBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(unsigned int), tempIndices, GL_STATIC_DRAW);

	// Vertex Array Object (VAO)
	glBindVertexArray(myVAO);
	glBindBuffer(GL_VERTEX_ARRAY, myVAO);

	// Vertex Buffer Object (VBO)
	glBindBuffer(GL_ARRAY_BUFFER, myVBO);
	glBufferData(GL_ARRAY_BUFFER, 24 * sizeof(float), tempVertices, GL_STATIC_DRAW);

	// Position and texture coordinates
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, false, sizeof(float) * 4, 0);

	myShader.Use();

	Matrix4x4 tempProjectionMatrix = Matrix4x4::Ortographic(0, aWindow.GetScreenWidth(), aWindow.GetScreenHeight(), 0);
	myShader.SetUniformMatrix4x4("ProjectionMatrix", tempProjectionMatrix);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, myTexture.GetID());

	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, tempIndices);
	//glDrawArrays(GL_TRIANGLES, 0, 6);

	glBindVertexArray(0);
	glActiveTexture(GL_TEXTURE0);
}