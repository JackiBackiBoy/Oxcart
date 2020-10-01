#ifndef TEXTURE_2D_HEADER
#define TEXTURE_2D_HEADER

#include <string>

class Texture2D
{
public:
	Texture2D() : myID(0), myType(""){};

private:
	unsigned int myID;
	std::string myType;
};
#endif