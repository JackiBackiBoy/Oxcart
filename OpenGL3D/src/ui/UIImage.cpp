#include "UIImage.h"

UIImage::UIImage()
{
}

void UIImage::Render(Window& aWindow)
{
	// A Quad
	float tempVertices[6][4] =
	{
		0.5f,  0.5f, 1.0f, 1.0f, // top right
		0.5f, -0.5f, 1.0f, 0.0f, // bottom right
		0.5f,  0.5f, 1.0f, 1.0f, // top right

	   -0.5f, -0.5f, 0.0f, 0.0f, // bottom left
	   -0.5f,  0.5f, 0.0f, 1.0f, // top left
	   0.5f, -0.5f, 1.0f, 0.0f, // bottom right
	};

	myShader.Use();

	glGenVertexArrays(1, &myVAO);
	glGenBuffers(1, &myVBO);
	glBindVertexArray(myVAO);
	glBindBuffer(GL_ARRAY_BUFFER, myVBO);
	glBufferData(GL_ARRAY_BUFFER, 24 * sizeof(float), tempVertices, GL_STATIC_DRAW);

	glBindBuffer(GL_VERTEX_ARRAY, myVAO);

	// Image position
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, false, sizeof(float) * 2, 0);

	// Texture coordinates
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, false, sizeof(float) * 2, (void*)(sizeof(float) * 2));

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, myTexture.GetID());

	glDrawArrays(GL_TRIANGLES, 0, 6);

	glBindVertexArray(0);
	glActiveTexture(GL_TEXTURE0);
}