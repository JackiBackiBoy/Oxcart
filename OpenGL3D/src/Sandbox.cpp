#include "core/Window.h"
#include "components/Camera.h"
#include "math/Math.h"
#include "math/Vector2D.h"
#include "math/Matrix4x4.h"

#include "input/Keyboard.h"
#include "input/Mouse.h"

#include "graphics/Shader.h"

#include "vendor/glm/glm.hpp"
#include "vendor/glm/gtc/matrix_transform.hpp"
#include "vendor/glm/gtc/type_ptr.hpp"
#include "ui/Character.h"

#include "data/Model.h"

#include <map>

#include <iostream>
#include "ft2build.h"
#include FT_FREETYPE_H

class Sandbox : public Window
{
public: 
	Sandbox(const std::string& aTitle, const int& aScreenWidth, const int& aScreenHeight)
		: Window::Window(aTitle, aScreenWidth, aScreenHeight) {}

	void OnStart() override
	{
		myLightingShader = new Shader("res/shaders/LightingShader.glsl");
		myLightCubeShader = new Shader("res/shaders/LightCubeShader.glsl");
		myGlyphShader = Shader("res/shaders/GlyphShader.glsl");

		glEnable(GL_DEPTH_TEST);
		glfwSetInputMode(myRawWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

		aModel = Model("res/models/Nanosuit/nanosuit.obj");

		glPixelStorei(GL_UNPACK_ALIGNMENT, 1); // disable byte-alingment

		// Load Freetype
		FT_Library tempFT;

		if (FT_Init_FreeType(&tempFT))
		{
			std::cout << "Error (Freetype): Could not initialize the FreeType library." << std::endl;
			exit(-1);
		}

		FT_Face tempFace;
		if (FT_New_Face(tempFT, "res/fonts/arial.ttf", 0, &tempFace))
		{
			std::cout << "Error (Freetype): Could not load the desired font." << std::endl;
			exit(-1);
		}

		FT_Set_Pixel_Sizes(tempFace, 0, 48);

		if (FT_Load_Char(tempFace, 'X', FT_LOAD_RENDER))
		{
			std::cout << "Error (Freetype): Failed to load the glyph." << std::endl;
			exit(-1);
		}

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
		}

		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

		FT_Done_Face(tempFace);
		FT_Done_FreeType(tempFT);
	}

	void OnUpdate(const float& aDeltaTime) override
	{
		// Forward
		if (Keyboard::IsKeyDown(KeyCode::W, myRawWindow))
		{
			myCameraPosition += myCameraFront * 2.0f * aDeltaTime;
		}
		// Backward
		if (Keyboard::IsKeyDown(KeyCode::S, myRawWindow))
		{
			myCameraPosition -= myCameraFront * 2.0f * aDeltaTime;
		}

		// Left
		if (Keyboard::IsKeyDown(KeyCode::A, myRawWindow))
		{
			myCameraPosition -= Vector3D::Normalize(Vector3D::CrossProduct(myCameraFront, myCameraUp)) * 2.0f * aDeltaTime;
		}
		// Right
		if (Keyboard::IsKeyDown(KeyCode::D, myRawWindow))
		{
			myCameraPosition += Vector3D::Normalize(Vector3D::CrossProduct(myCameraFront, myCameraUp)) * 2.0f * aDeltaTime;
		}

		// Up
		if (Keyboard::IsKeyDown(KeyCode::Space, myRawWindow))
		{
			myCameraPosition.y += 2.0f * aDeltaTime;
		}
		// Down
		if (Keyboard::IsKeyDown(KeyCode::LeftControl, myRawWindow))
		{
			myCameraPosition.y -= 2.0f * aDeltaTime;
		}

		Vector2D tempCurrentMousePosition = Mouse::GetPosition(myRawWindow);

		float tempDeltaMouseX = tempCurrentMousePosition.x - myLastMousePosition.x;
		float tempDeltaMouseY = myLastMousePosition.y - tempCurrentMousePosition.y;
		myLastMousePosition = tempCurrentMousePosition;

		myYaw += tempDeltaMouseX * 0.1f;
		myPitch += tempDeltaMouseY * 0.1f;

		if (myPitch > 89.0f)
		{
			myPitch = 89.0f;
		}
		if (myPitch < -89.0f)
		{
			myPitch= -89.0f;
		}

		Vector3D tempDirection;
		tempDirection.x = cos(Math::ToRadians(myYaw)) * cos(Math::ToRadians(myPitch));
		tempDirection.y = sin(Math::ToRadians(myPitch));
		tempDirection.z = sin(Math::ToRadians(myYaw)) * cos(Math::ToRadians(myPitch));
		myCameraFront = Vector3D::Normalize(tempDirection);
	}

	void OnRender(const float& aDeltaTime) override
	{
		glUseProgram(myLightingShader->GetID());
		glUniform3f(glGetUniformLocation(myLightingShader->GetID(), "light.position"), myCameraPosition.x, myCameraPosition.y, myCameraPosition.z);
		glUniform3f(glGetUniformLocation(myLightingShader->GetID(), "directionalLight.direction"), -1.0f, -0.23f, -0.9f);
		glUniform1f(glGetUniformLocation(myLightingShader->GetID(), "light.cutOff"), cos(Math::ToRadians(12.5f)));
		glUniform1f(glGetUniformLocation(myLightingShader->GetID(), "light.outerCutOff"), cos(Math::ToRadians(17.5f)));

		// Spotlight
		glUniform3f(glGetUniformLocation(myLightingShader->GetID(), "spotLight.position"), myCameraPosition.x, myCameraPosition.y, myCameraPosition.z);
		glUniform3f(glGetUniformLocation(myLightingShader->GetID(), "spotLight.direction"), myCameraFront.x, myCameraFront.y, myCameraFront.z);
		glUniform1f(glGetUniformLocation(myLightingShader->GetID(), "spotLight.cutOff"), cos(Math::ToRadians(12.5f)));
		glUniform1f(glGetUniformLocation(myLightingShader->GetID(), "spotLight.outerCutOff"), cos(Math::ToRadians(17.5f)));
		glUniform3f(glGetUniformLocation(myLightingShader->GetID(), "spotLight.ambient"), 0.1f, 0.1f, 0.1f);
		glUniform3f(glGetUniformLocation(myLightingShader->GetID(), "spotLight.diffuse"), 0.8f, 0.8f, 0.8f);
		glUniform3f(glGetUniformLocation(myLightingShader->GetID(), "spotLight.specular"), 1.0f, 1.0f, 1.0f);
		glUniform1f(glGetUniformLocation(myLightingShader->GetID(), "spotLight.constantTerm"), 1.0f);
		glUniform1f(glGetUniformLocation(myLightingShader->GetID(), "spotLight.linearTerm"), 0.09f);
		glUniform1f(glGetUniformLocation(myLightingShader->GetID(), "spotLight.quadraticTerm"), 0.032f);

		glUniform3f(glGetUniformLocation(myLightingShader->GetID(), "ViewPosition"), myCameraPosition.x, myCameraPosition.y, myCameraPosition.z);


		glUniform1i(glGetUniformLocation(myLightingShader->GetID(), "material.diffuse"), 0);
		glUniform1i(glGetUniformLocation(myLightingShader->GetID(), "material.specular"), 1);
		glUniform1f(glGetUniformLocation(myLightingShader->GetID(), "material.shininess"), 32.0f);
		glUniform3f(glGetUniformLocation(myLightingShader->GetID(), "directionalLight.ambient"), 0.2f, 0.2f, 0.2f);
		glUniform3f(glGetUniformLocation(myLightingShader->GetID(), "directionalLight.diffuse"), 0.5f, 0.5f, 0.5f);
		glUniform3f(glGetUniformLocation(myLightingShader->GetID(), "directionalLight.specular"), 1.0f, 1.0f, 1.0f);
		glUniform1f(glGetUniformLocation(myLightingShader->GetID(), "light.constantTerm"), 1.0f);
		glUniform1f(glGetUniformLocation(myLightingShader->GetID(), "light.linearTerm"), 0.09f);
		glUniform1f(glGetUniformLocation(myLightingShader->GetID(), "light.quadraticTerm"), 0.032f);

		// Model Matrix
		Matrix4x4 tempModelMatrix = Matrix4x4::Identity();
		tempModelMatrix = Matrix4x4::Rotate(tempModelMatrix, 0.0f, { 1.0f, 1.0f, 1.0f });

		// View Matrix
		Matrix4x4 tempViewMatrix;
		tempViewMatrix = Matrix4x4::LookAt(myCameraPosition, myCameraPosition + myCameraFront, myCameraUp);

		// Projection Matrix
		Matrix4x4 tempProjectionMatrix;
		tempProjectionMatrix = Matrix4x4::Perspective(Math::ToRadians(70.0f), GetAspectRatio(), 0.1f, 100.0f);

		glUniformMatrix4fv(glGetUniformLocation(myLightingShader->GetID(), "ModelMatrix"), 1, false, tempModelMatrix.GetValuePtr());
		glUniformMatrix4fv(glGetUniformLocation(myLightingShader->GetID(), "ViewMatrix"), 1, GL_FALSE, tempViewMatrix.GetValuePtr());
		glUniformMatrix4fv(glGetUniformLocation(myLightingShader->GetID(), "ProjectionMatrix"), 1, GL_FALSE, tempProjectionMatrix.GetValuePtr());

		aModel.Render(*myLightingShader);

		// Font rendering
		glUseProgram(myGlyphShader.GetID());
		tempProjectionMatrix = Matrix4x4::Ortographic(0.0f, myScreenWidth, 0.0f, myScreenHeight);
		glUniformMatrix4fv(glGetUniformLocation(myGlyphShader.GetID(), "ProjectionMatrix"), 1, false, tempProjectionMatrix.GetValuePtr());
	}

private:
	std::map<char, Character> myCharacters;

	Vector2D myLastMousePosition;

	Vector3D myCameraPosition = { 0.0f, 0.0f, 3.0f };
	Vector3D myCameraUp = { 0.0f, 1.0f, 0.0f };
	Vector3D myCameraFront = { 0.0f, 0.0f, -1.0f };

	unsigned int myVAO;
	unsigned int myVBO;

	float myPitch;
	float myYaw = -90.0f;
	float myRoll;

	Shader* myLightingShader;
	Shader* myLightCubeShader;
	Shader myGlyphShader;

	Vector3D myLightPosition = { 1.2f, 1.0f, 2.0f };
	Model aModel;
};

Window* BuildWindow()
{
	return new Sandbox("Oxcart 3D | Made by Jack Henrikson", 1920, 1080);
}