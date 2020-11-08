#ifndef TRIANGLE_HEADER
#define TRIANGLE_HEADER

#include "Vector3D.h"

struct OX_API Triangle
{
	Triangle() {};

	Vector3D vertices[3];
	Vector3D normal;
};
#endif