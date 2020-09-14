#include "core/Window.h"
#include "components/Camera.h"
#include "math/Math.h"
#include "math/Vector2D.h"
#include "math/Matrix4x4.h"

#include "input/Keyboard.h"
#include "input/Mouse.h"

#include "graphics/Shader.h"
#include "graphics/Texture.h"

class Sandbox : public Window
{
public: 
	Sandbox(const std::string& aTitle, const int& aScreenWidth, const int& aScreenHeight)
		: Window::Window(aTitle, aScreenWidth, aScreenHeight) {}

	void OnStart() override
	{
		myCamera = new Camera();
		myCamera->GetTransform().Position() = { 0.0f, 0.0f, 3.0f };
		myCamera->GetTransform().Rotation() = { 0.0f, 0.0f, -1.0f };

		myLightingShader = new Shader("res/shaders/LightingShader.glsl");
		myLightCubeShader = new Shader("res/shaders/LightCubeShader.glsl");

		myWoodBoxTexture = new Texture("res/textures/wood_box.png");
		myWoodBoxSpecularMap = new Texture("res/textures/wood_box_specular_map.png");

		float tempVertices[] =
		{
			// positions          // normals           // texture coords
			-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
			 0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 0.0f,
			 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
			 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
			-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 1.0f,
			-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,

			-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,
			 0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 0.0f,
			 0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
			 0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
			-0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 1.0f,
			-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,

			-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
			-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
			-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
			-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
			-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
			-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

			 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
			 0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
			 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
			 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
			 0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
			 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

			-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,
			 0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 1.0f,
			 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
			 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
			-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 0.0f,
			-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,

			-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f,
			 0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 1.0f,
			 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
			 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
			-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 0.0f,
			-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f
		};

		Vector3D cubePositions[] =
		{
			Vector3D(0.0f,  0.0f,  0.0f),
			Vector3D(2.0f,  5.0f, -15.0f),
			Vector3D(-1.5f, -2.2f, -2.5f),
			Vector3D(-3.8f, -2.0f, -12.3f),
			Vector3D(2.4f, -0.4f, -3.5f),
			Vector3D(-1.7f,  3.0f, -7.5f),
			Vector3D(1.3f, -2.0f, -2.5f),
			Vector3D(1.5f,  2.0f, -2.5f),
			Vector3D(1.5f,  0.2f, -1.5f),
			Vector3D(-1.3f,  1.0f, -1.5f)
		};

		Vector3D pointLightPositions[] =
		{
			Vector3D(0.7f,  0.2f,  2.0f),
			Vector3D(2.3f, -3.3f, -4.0f),
			Vector3D(-4.0f,  2.0f, -12.0f),
			Vector3D(0.0f,  0.0f, -3.0f)
		};
	}

	void OnUpdate(float aDeltaTime) override
	{
		// Forward
		if (Keyboard::IsKeyDown(KeyCode::W, myRawWindow))
		{
			myCamera->GetTransform().Position() += myCamera->GetTransform().GetForward() * 2.0f * aDeltaTime;
		}
		// Backward
		if (Keyboard::IsKeyDown(KeyCode::S, myRawWindow))
		{
			myCamera->GetTransform().Position() -= myCamera->GetTransform().GetForward() * 2.0f * aDeltaTime;
		}

		// Left
		if (Keyboard::IsKeyDown(KeyCode::A, myRawWindow))
		{
			myCamera->GetTransform().Position() -= Vector3D::Normalize(Vector3D::CrossProduct(myCamera->GetTransform().GetForward(), Vector3D::Up)) * 2.0f * aDeltaTime;
		}
		// Right
		if (Keyboard::IsKeyDown(KeyCode::D, myRawWindow))
		{
			myCamera->GetTransform().Position() += Vector3D::Normalize(Vector3D::CrossProduct(myCamera->GetTransform().GetForward(), Vector3D::Up)) * 2.0f * aDeltaTime;
		}

		// Up
		if (Keyboard::IsKeyDown(KeyCode::Space, myRawWindow))
		{
			myCamera->GetTransform().Position().y += 2.0f * aDeltaTime;
		}
		// Down
		if (Keyboard::IsKeyDown(KeyCode::LeftControl, myRawWindow))
		{
			myCamera->GetTransform().Position().y -= 2.0f * aDeltaTime;
		}

		Vector2D tempCurrentMousePosition = Mouse::GetPosition(myRawWindow);

		float tempDeltaMouseX = tempCurrentMousePosition.x - myLastMousePosition.x;
		float tempDeltaMouseY = myLastMousePosition.y - tempCurrentMousePosition.y;
		myLastMousePosition = tempCurrentMousePosition;

		myCamera->Yaw() += tempDeltaMouseX * 0.1f;
		myCamera->Pitch() += tempDeltaMouseY * 0.1f;

		if (myCamera->Pitch() > 89.0f)
		{
			myCamera->Pitch() = 89.0f;
		}
		if (myCamera->Pitch() < -89.0f)
		{
			myCamera->Pitch() = -89.0f;
		}

		Vector3D tempDirection;
		tempDirection.x = cos(Math::ToRadians(myCamera->Yaw())) * cos(Math::ToRadians(myCamera->Pitch()));
		tempDirection.y = sin(Math::ToRadians(myCamera->Pitch()));
		tempDirection.z = sin(Math::ToRadians(myCamera->Yaw())) * cos(Math::ToRadians(myCamera->Pitch()));
		myCamera->GetTransform().Rotation() = Vector3D::Normalize(tempDirection);
	}

	void OnRender(float aDeltaTime) override
	{
		glUseProgram(myLightingShader->GetID());
		glUniform3f(glGetUniformLocation(myLightingShader->GetID(), "light.position"), myCamera->GetTransform().Position().x, myCamera->GetTransform().Position().y, myCamera->GetTransform().Position().z);
		glUniform3f(glGetUniformLocation(myLightingShader->GetID(), "directionalLight.direction"), -1.0f, -0.23f, -0.9f);
		glUniform1f(glGetUniformLocation(myLightingShader->GetID(), "light.cutOff"), cos(Math::ToRadians(12.5f)));
		glUniform1f(glGetUniformLocation(myLightingShader->GetID(), "light.outerCutOff"), cos(Math::ToRadians(17.5f)));

		// Spotlight
		glUniform3f(glGetUniformLocation(myLightingShader->GetID(), "spotLight.position"), myCamera->GetTransform().Position().x, myCamera->GetTransform().Position().y, myCamera->GetTransform().Position().z);
		glUniform3f(glGetUniformLocation(myLightingShader->GetID(), "spotLight.direction"), myCamera->GetTransform().GetForward().x, myCamera->GetTransform().GetForward().y, myCamera->GetTransform().GetForward().z);
		glUniform1f(glGetUniformLocation(myLightingShader->GetID(), "spotLight.cutOff"), cos(Math::ToRadians(12.5f)));
		glUniform1f(glGetUniformLocation(myLightingShader->GetID(), "spotLight.outerCutOff"), cos(Math::ToRadians(17.5f)));
		glUniform3f(glGetUniformLocation(myLightingShader->GetID(), "spotLight.ambient"), 0.1f, 0.1f, 0.1f);
		glUniform3f(glGetUniformLocation(myLightingShader->GetID(), "spotLight.diffuse"), 0.8f, 0.8f, 0.8f);
		glUniform3f(glGetUniformLocation(myLightingShader->GetID(), "spotLight.specular"), 1.0f, 1.0f, 1.0f);
		glUniform1f(glGetUniformLocation(myLightingShader->GetID(), "spotLight.constantTerm"), 1.0f);
		glUniform1f(glGetUniformLocation(myLightingShader->GetID(), "spotLight.linearTerm"), 0.09f);
		glUniform1f(glGetUniformLocation(myLightingShader->GetID(), "spotLight.quadraticTerm"), 0.032f);

		glUniform3f(glGetUniformLocation(myLightingShader->GetID(), "ViewPosition"), myCamera->GetTransform().Position().x, myCamera->GetTransform().Position().y, myCamera->GetTransform().Position().z);

		myWoodBoxTexture->Bind(0);
		myWoodBoxSpecularMap->Bind(1);

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
		Matrix4x4 tempModelMatrix = Matrix4x4(1.0f);
		tempModelMatrix = Matrix4x4::Rotate(tempModelMatrix, 0.0f, { 1.0f, 1.0f, 1.0f });
		tempModelMatrix = glm::rotate(tempModelMatrix, 0.0f, glm::vec3(1.0f, 1.0f, 1.0f));

		// View Matrix
		glm::mat4 tempViewMatrix;
		tempViewMatrix = glm::lookAt(myCamera->GetTransform().Position(), myCamera->GetTransform().Position() + myCameraFront, myCameraUp);

		// Projection Matrix
		glm::mat4 tempProjectionMatrix;
		tempProjectionMatrix = glm::perspective(Math::ToRadians(45.0f), myAspectRatio, 0.1f, 100.0f);

		glUniformMatrix4fv(glGetUniformLocation(myLightingShader.GetID(), "ModelMatrix"), 1, GL_FALSE, glm::value_ptr(tempModelMatrix));
		glUniformMatrix4fv(glGetUniformLocation(myLightingShader.GetID(), "ViewMatrix"), 1, GL_FALSE, glm::value_ptr(tempViewMatrix));
		glUniformMatrix4fv(glGetUniformLocation(myLightingShader.GetID(), "ProjectionMatrix"), 1, GL_FALSE, glm::value_ptr(tempProjectionMatrix));

		glBindVertexArray(myVAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		for (size_t i = 0; i < 10; i++)
		{
			glm::mat4 model = glm::mat4(1.0f);
			model = glm::translate(model, cubePositions[i]);
			float angle = 20.0f * i;
			model = glm::rotate(model, Math::ToRadians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
			glUniformMatrix4fv(glGetUniformLocation(myLightingShader.GetID(), "ModelMatrix"), 1, GL_FALSE, glm::value_ptr(model));

			glDrawArrays(GL_TRIANGLES, 0, 36);
		}

		// Draw the scene light
		glUseProgram(myLightCubeShader.GetID());
		glUniformMatrix4fv(glGetUniformLocation(myLightCubeShader.GetID(), "ProjectionMatrix"), 1, GL_FALSE, glm::value_ptr(tempProjectionMatrix));
		glUniformMatrix4fv(glGetUniformLocation(myLightCubeShader.GetID(), "ViewMatrix"), 1, GL_FALSE, glm::value_ptr(tempViewMatrix));
		tempModelMatrix = glm::mat4(1.0f);
		tempModelMatrix = glm::translate(tempModelMatrix, myLightPosition);
		tempModelMatrix = glm::scale(tempModelMatrix, glm::vec3(0.2f));
		glUniformMatrix4fv(glGetUniformLocation(myLightCubeShader.GetID(), "ModelMatrix"), 1, GL_FALSE, glm::value_ptr(tempModelMatrix));

		glBindVertexArray(myLightVAO);

		for (size_t i = 0; i < 4; i++)
		{

		}

		glDrawArrays(GL_TRIANGLES, 0, 36);
	}

private:
	Camera* myCamera;
	Vector2D myLastMousePosition;

	Shader* myLightingShader;
	Shader* myLightCubeShader;

	Texture* myWoodBoxTexture;
	Texture* myWoodBoxSpecularMap;
};

Window* BuildWindow()
{
	return new Sandbox("Oxcart 3D | Made by Jack Henrikson", 1920, 1080);
}