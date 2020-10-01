#ifndef MESH_HEADER
#define MESH_HEADER

#include <vector>
#include "Vertex.h"
#include "Texture2D.h"
#include "graphics/Shader.h"

class Mesh
{
public:
	Mesh(const std::vector<Vertex>& someVertices, const std::vector<unsigned int>& someIndices, const std::vector<Texture2D>& someTextures);
	~Mesh() {};

	void Render(Shader& aShader) const;

private:
	std::vector<Vertex> myVertices;
	std::vector<unsigned int> myIndices;
	std::vector<Texture2D> myTextures;

	unsigned int myVAO;
	unsigned int myVBO;
	unsigned int myEBO;
};
#endif