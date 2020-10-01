#ifndef TRIANGLE_HEADER
#define TRIANGLE_HEADER

#include "Vector3D.h"

struct Triangle
{
	Triangle() {};

	Vector3D vertices[3];
	Vector3D normal;
};
#endif