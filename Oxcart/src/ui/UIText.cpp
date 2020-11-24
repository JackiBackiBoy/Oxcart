#include "UIText.h"
#include "math/Matrix4x4.h"
#include <iostream>
#include "GL/glew.h"
#include "ft2build.h"
#include FT_FREETYPE_H
#include "freetype/ftlcdfil.h"


UIText::UIText(const std::string& someText, const Vector2D& aPosition, const Color& aColor)
	: myText(someText), myColor(aColor), myScale({ 1, 1 }), UIElement(aPosition)
{
	glGenVertexArrays(1, &myVAO);
	glGenBuffers(1, &myVBO);

	myGlyphShader = Shader("res/shaders/GlyphShader.glsl");

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1); // disable byte-alingment

	// Load Freetype
	FT_Library tempFT;

	if (FT_Init_FreeType(&tempFT))
	{
		std::cout << "Error (Freetype): Could not initialize the FreeType library." << std::endl;
		exit(-1);
	}

	FT_Face tempFace;
	if (FT_New_Face(tempFT, "res/fonts/segoeui.ttf", 0, &tempFace))
	{
		std::cout << "Error (Freetype): Could not load the desired font." << std::endl;
		exit(-1);
	}
	else
	{
		FT_Set_Pixel_Sizes(tempFace, 0, 10);
		FT_Library_SetLcdFilter(tempFT, FT_LcdFilter::FT_LCD_FILTER_DEFAULT);

		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

		for (unsigned char i = 0; i < 128; i++)
		{
			// Load the character glyph
			if (FT_Load_Char(tempFace, i, FT_LOAD_RENDER))
			{
				std::cout << "Error (Freetype): Failed to load the glyph." << std::endl;
				continue;
			}

			unsigned int tempTexture;
			glGenTextures(1, &tempTexture);
			glBindTexture(GL_TEXTURE_2D, tempTexture);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, tempFace->glyph->bitmap.width, tempFace->glyph->bitmap.rows, 0, GL_RED, GL_UNSIGNED_BYTE, tempFace->glyph->bitmap.buffer);

			// Texture coords
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

			Character tempCharacter = { tempTexture, tempFace->glyph->bitmap.width, tempFace->glyph->bitmap.rows, tempFace->glyph->bitmap_left, tempFace->glyph->bitmap_top, tempFace->glyph->advance.x };
			myCharacters.insert(std::pair<char, Character>(i, tempCharacter));

			// Check if the character is the largest
			if (tempCharacter.sizeY > myLargestCharacterSize)
			{
				myLargestCharacterSize = tempCharacter.bearingY;
			}
		}
	}

	FT_Done_Face(tempFace);
	FT_Done_FreeType(tempFT);
}

int UIText::GetTextWidth()
{
	return myTextWidth;
}

int UIText::GetTextHeight()
{
	return myLargestCharacterSize;
}

Vector2D& UIText::Scale()
{
	return myScale;
}

void UIText::Render(Window& aWindow)
{
	//glBlendFunc(GL_SRC1_COLOR, GL_ONE_MINUS_SRC1_COLOR);

	// Font rendering
	glUseProgram(myGlyphShader.GetID());
	Matrix4x4 tempProjectionMatrix = Matrix4x4::Ortographic(0.0f, aWindow.GetScreenWidth(), aWindow.GetScreenHeight(), 0.0f);
	glUniformMatrix4fv(glGetUniformLocation(myGlyphShader.GetID(), "ProjectionMatrix"), 1, false, tempProjectionMatrix.GetValuePtr());

	glBindVertexArray(myVAO);
	glBindBuffer(GL_ARRAY_BUFFER, myVBO);

	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	Vector4D tempNormalizedColor = Color::Normalize(myColor);
	myGlyphShader.SetUniform4f("TextColor", tempNormalizedColor.x, tempNormalizedColor.y, tempNormalizedColor.z, tempNormalizedColor.w);
	glActiveTexture(GL_TEXTURE0);
	glBindVertexArray(myVAO);

	std::string::const_iterator tempIterator;

	int tempX = (int)myPosition.x;
	int tempY = (int)myPosition.y;

	myTextWidth = 0;
	myTextHeight = 0;

	for (tempIterator = myText.begin(); tempIterator != myText.end(); tempIterator++)
	{
		Character tempCharacter = myCharacters[*tempIterator];

		float tempXPosition = tempX - tempCharacter.bearingX * myScale.x;
		float tempYPosition = tempY + myLargestCharacterSize * myScale.y - tempCharacter.bearingY * myScale.y;

		float tempWidth = tempCharacter.sizeX * myScale.x;
		float tempHeight = tempCharacter.sizeY * myScale.y;

		myTextWidth += (tempCharacter.advanceOffset >> 6) * myScale.x;
		myTextHeight = tempHeight * myScale.y;

		float tempVertices[6][4] =
		{
			{ tempXPosition, tempYPosition + tempHeight, 0.0f, 1.0f },
			{ tempXPosition, tempYPosition, 0.0f, 0.0f },
			{ tempXPosition + tempWidth, tempYPosition, 1.0f, 0.0f },

			{ tempXPosition, tempYPosition + tempHeight, 0.0f, 1.0f },
			{ tempXPosition + tempWidth, tempYPosition, 1.0f, 0.0f },
			{ tempXPosition + tempWidth, tempYPosition + tempHeight, 1.0f, 1.0f }
		};

		glBindTexture(GL_TEXTURE_2D, tempCharacter.textureID);
		glBindBuffer(GL_ARRAY_BUFFER, myVBO);
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(tempVertices), tempVertices);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		// Render the quad
		glDrawArrays(GL_TRIANGLES, 0, 6);

		// advance cursors for the next glyph
		// bitshift trick to get value in pixels (2^6 = 64)
		// this is done because the advance number is number of
		// 1/64 pixels.
		tempX += (tempCharacter.advanceOffset >> 6) * myScale.x;
	}

	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);
}