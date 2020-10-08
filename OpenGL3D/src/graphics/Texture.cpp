#include "Texture.h"
#include "vendor/stb_image/stb_image.h"
#include "GL/glew.h"

Texture::Texture(const std::string& aFilePath)
{
	stbi_set_flip_vertically_on_load(1);
	unsigned char* tempTextureData = stbi_load(aFilePath.c_str(), &myWidth, &myHeight, &myChannelCount, 4);

	glGenTextures(1, &myID);
	glBindTexture(GL_TEXTURE_2D, myID);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, myWidth, myHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, tempTextureData);
	glGenerateMipmap(GL_TEXTURE_2D);

	stbi_image_free(tempTextureData);

	glBindTexture(GL_TEXTURE0, 0);
}

Texture::~Texture()
{

}

void Texture::Bind(const unsigned int& aTextureSlot) const
{
	glActiveTexture(GL_TEXTURE0 + aTextureSlot);
	glBindTexture(GL_TEXTURE_2D, myID);
}