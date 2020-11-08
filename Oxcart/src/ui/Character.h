#ifndef CHARACTER_HEADER
#define CHARACTER_HEADER

#include "core/Window.h"

struct OX_API Character
{
	unsigned int textureID;
	int sizeX;
	int sizeY;
	int bearingX;
	int bearingY;
	unsigned int advanceOffset;
};
#endif