#ifndef VERTEX_HEADER
#define VERTEX_HEADER

#include "math/Vector3D.h"
#include "math/Vector2D.h"

struct Vertex
{
	Vertex() : position({ 0, 0, 0 }), normal({ 0, 0, 0 }), textureCoords({ 0, 0 }) {};
	Vertex(const Vector3D& aPosition, const Vector3D& aNormal, const Vector2D& someTextureCoords) : position(aPosition), normal(aNormal), textureCoords(someTextureCoords) {};
	~Vertex() {};

	Vector3D position;
	Vector3D normal;
	Vector2D textureCoords;
};
#endif