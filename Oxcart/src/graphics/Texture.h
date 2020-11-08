#ifndef TEXTURE_HEADER
#define TEXTURE_HEADER

#include <string>
#include "core/Core.h"

class OX_API Texture
{
public:
	Texture() {};
	Texture(const std::string& aFilePath);
	~Texture();

	void Bind(const unsigned int& aTextureSlot) const;

	inline int GetWidth() const { return myWidth; }
	inline int GetHeight() const { return myHeight; }
	inline unsigned int GetID() const { return myID; }

private:
	int myWidth;
	int myHeight;
	int myChannelCount;

	unsigned int myID;
};
#endif