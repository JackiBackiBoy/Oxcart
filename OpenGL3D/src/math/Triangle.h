#ifndef TRIANGLE_HEADER
#define TRIANGLE_HEADER

#include "Vector3D.h"

struct Triangle
{
	Triangle() : vertices(), normal({ 0, 0, 0 }) {};

	Vector3D vertices[3];
	Vector3D normal;
};
#endif