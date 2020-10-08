#ifndef TEXTURE_HEADER
#define TEXTURE_HEADER

#include <string>

class Texture
{
public:
	Texture() {};
	Texture(const std::string& aFilePath);
	~Texture();

	void Bind(const unsigned int& aTextureSlot) const;

	inline unsigned int GetID() const { return myID; }

private:
	int myWidth;
	int myHeight;
	int myChannelCount;

	unsigned int myID;
};
#endif