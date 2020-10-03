#include "Mesh.h"
#include "GL/glew.h"

Mesh::Mesh(const std::vector<Vertex>& someVertices, const std::vector<unsigned int>& someIndices, const std::vector<Texture2D>& someTextures)
	: myVertices(someVertices), myIndices(someIndices), myTextures(someTextures)
{
	glGenVertexArrays(1, &myVAO);
	glGenBuffers(1, &myVBO);
	glGenBuffers(1, &myEBO);

	// VAO
	// Vertex positions
	glBindVertexArray(myVAO);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);

	// Normals
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));

	// Texture coords
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, textureCoords));

	// VBO
	glBindBuffer(GL_ARRAY_BUFFER, myVBO);
	glBufferData(GL_ARRAY_BUFFER, myVertices.size() * sizeof(Vertex), &myVertices[0], GL_STATIC_DRAW);

	// EBO
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, myEBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, myIndices.size() * sizeof(unsigned int), &myIndices[0], GL_STATIC_DRAW);

	glBindVertexArray(0);
}

void Mesh::Render(Shader& aShader)
{
	unsigned int tempDiffuseNumber = 1;
	unsigned int tempSpecularNumber = 1;

	for (unsigned int i = 0; i < myTextures.size(); i++)
	{
		glActiveTexture(GL_TEXTURE0 + i);

		std::string tempNumber;
		std::string tempName = myTextures[i].type;

		if (tempName == "diffuse")
		{
			tempNumber = std::to_string(tempDiffuseNumber++);
		}
		else if (tempName == "specular")
		{
			tempNumber = std::to_string(tempSpecularNumber++);
		}

		glUniform1i(glGetUniformLocation(aShader.GetID(), ("material." +  tempName).c_str()), i);
		glBindTexture(GL_TEXTURE_2D, myTextures[i].ID);
	}

	glBindVertexArray(myVAO);
	glDrawElements(GL_TRIANGLES, myIndices.size(), GL_UNSIGNED_INT, (void*)0);
	glBindVertexArray(0);

	glActiveTexture(GL_TEXTURE0);
}