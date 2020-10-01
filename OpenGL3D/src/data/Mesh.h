#ifndef MESH_HEADER
#define MESH_HEADER

#include <vector>
#include "Vertex.h"
#include "Texture2D.h"

class Mesh
{
public:
	Mesh();
	~Mesh();

private:
	std::vector<Vertex> myVertices;
	std::vector<unsigned int> myIndices;
	std::vector<Texture2D> myTextures;
};
#endif