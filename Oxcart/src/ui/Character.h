#ifndef CHARACTER_HEADER
#define CHARACTER_HEADER

struct Character
{
	unsigned int textureID;
	int sizeX;
	int sizeY;
	int bearingX;
	int bearingY;
	unsigned int advanceOffset;
};
#endif