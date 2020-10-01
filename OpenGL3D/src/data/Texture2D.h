#ifndef TEXTURE_2D_HEADER
#define TEXTURE_2D_HEADER

#include <string>

struct Texture2D
{
	Texture2D() : ID(0), type(""), path("") {};

	unsigned int ID;
	std::string type;
	std::string path;
};
#endif