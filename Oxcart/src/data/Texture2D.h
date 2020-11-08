#ifndef TEXTURE_2D_HEADER
#define TEXTURE_2D_HEADER

#include <string>
#include "core/Core.h"

struct OX_API Texture2D
{
	Texture2D() : ID((unsigned int)0), type(""), path("") {};

	unsigned int ID;
	std::string type;
	std::string path;
};
#endif