#include "UIImage.h"

UIImage::UIImage()
{
}

void UIImage::Render(Window& aWindow)
{
	// A Quad
	float tempVertices[6][4] =
	{
	    0,  0, 0.0f, 1.0f, // top left
	    0, 200, 0.0,  0.0f, // bottom left
		200, 200, 1.0,  0.0f, // bottom right

	    0,  0, 0.0f, 1.0f, // top left
		200, 200, 1.0,  0.0f, // bottom right
		200, 0, 1.0f, 1.0f, // top right
	};

	myShader.Use();

	Matrix4x4 tempProjectionMatrix = Matrix4x4::Ortographic(0, aWindow.GetScreenWidth(), aWindow.GetScreenHeight(), 0);
	myShader.SetUniformMatrix4x4("ProjectionMatrix", tempProjectionMatrix);

	glGenVertexArrays(1, &myVAO);
	glGenBuffers(1, &myVBO);
	glBindVertexArray(myVAO);
	glBindBuffer(GL_ARRAY_BUFFER, myVBO);
	glBufferData(GL_ARRAY_BUFFER, 24 * sizeof(float), tempVertices, GL_STATIC_DRAW);

	glBindBuffer(GL_VERTEX_ARRAY, myVAO);

	// Position and texture coordinates
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, false, sizeof(float) * 4, 0);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, myTexture.GetID());

	glDrawArrays(GL_TRIANGLES, 0, 6);

	glBindVertexArray(0);
	glActiveTexture(GL_TEXTURE0);
}