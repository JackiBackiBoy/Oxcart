#include "UIButton.h"
#include "input/Mouse.h"
#include <iostream>

UIButton::UIButton(const std::string& aText, const Vector2D& aPosition, const int& aWidth, const int& aHeight, const Color& aTextColor, const Color& aButtonColor)
	: myRectangle(aPosition, aWidth, aHeight), UIElement(aPosition)
{
	myButtonBackground = UIImage(Texture("res/textures/pixel.png"), aPosition, { (float)myRectangle.GetWidth(), (float)myRectangle.GetHeight() }, aButtonColor);
	myButtonBackground.myShader = Shader("res/shaders/UIImageShader.glsl");
	myText = UIText(aText, aPosition, aTextColor);
	myText.Position() = { aPosition.x + (myButtonBackground.GetWidth() / 2), 0 };

	myVertices =
	{
		myPosition.x, myPosition.y,
		myPosition.x, myPosition.y + (float)myRectangle.GetHeight(),
		myPosition.x + (float)myRectangle.GetWidth(), myPosition.y + (float)myRectangle.GetHeight(),
		myPosition.x + (float)myRectangle.GetWidth(), myPosition.x
	};

	myIndices = { 0, 1, 2, 0, 2, 3 };

	glGenVertexArrays(1, &myVAO);
	glGenBuffers(1, &myVBO);
	glGenBuffers(1, &myEBO);
}

void UIButton::Render(Window& aWindow)
{
	if (Mouse::IsButtonDownOnce(MouseButton::Left, aWindow.GetRawWindow()) && myRectangle.Contains(Mouse::GetPosition(aWindow.GetRawWindow())))
	{
		myOnClick();
	}

	myPosition = aWindow.GetCenter() - Vector2D(myRectangle.GetWidth() / 2, myRectangle.GetHeight() / 2);
	myRectangle.Position() = aWindow.GetCenter() - Vector2D(myRectangle.GetWidth() / 2, myRectangle.GetHeight() / 2);

	myButtonBackground.Position() = aWindow.GetCenter() - Vector2D(myRectangle.GetWidth() / 2, myRectangle.GetHeight() / 2);
	myButtonBackground.Render(aWindow);

	//myText.Position() = { myPosition.x + (float)myRectangle.GetWidth() / 2 - myText.GetTextWidth() / 2, myPosition.y + (float)myRectangle.GetHeight() / 2 - myText.GetTextHeight() / 2 };
	myText.Position() = aWindow.GetCenter() - Vector2D(myText.GetTextWidth() / 2, myText.GetTextHeight() / 2);
	myText.Render(aWindow);

	// Show hover effect
	if (myRectangle.Contains(Mouse::GetPosition(aWindow.GetRawWindow())))
	{
		// Update the vertices in case the image has either been moved or resized
		myVertices[0] = myPosition.x;
		myVertices[1] = myPosition.y;

		myVertices[2] = myPosition.x;
		myVertices[3] = myPosition.y + myRectangle.GetHeight();

		myVertices[4] = myPosition.x + myRectangle.GetWidth();
		myVertices[5] = myPosition.y + myRectangle.GetHeight();

		myVertices[6] = myPosition.x + myRectangle.GetWidth();
		myVertices[7] = myPosition.y;

		myHoverShader.Use();

		// Binding
		// Element Buffer Object (EBO)
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, myEBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(unsigned int), &myIndices[0], GL_STATIC_DRAW);

		// Vertex Array Object (VAO)
		glBindVertexArray(myVAO);
		glBindBuffer(GL_VERTEX_ARRAY, myVAO);

		// Vertex Buffer Object (VBO)
		glBindBuffer(GL_ARRAY_BUFFER, myVBO);
		glBufferData(GL_ARRAY_BUFFER, 8 * sizeof(float), &myVertices[0], GL_STATIC_DRAW);

		// Position and texture coordinates
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 2, GL_FLOAT, false, sizeof(float) * 2, 0);

		Matrix4x4 tempProjectionMatrix = Matrix4x4::Ortographic(0, aWindow.GetScreenWidth(), aWindow.GetScreenHeight(), 0);
		myHoverShader.SetUniformMatrix4x4("ProjectionMatrix", tempProjectionMatrix);

		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, &myIndices[0]);
	}
}