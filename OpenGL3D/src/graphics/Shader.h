#ifndef SHADER_HEADER
#define SHADER_HEADER

#include <string>

class Shader
{
public:
	Shader(const std::string& aFilePath);
	~Shader();

	inline unsigned int GetVertexShaderID() const { return myVertexShaderID; }
	inline unsigned int GetFragmentShaderID() const { return myFragmentShaderID; }
	inline unsigned int GetID() const { return myID; }

private:
	void ParseShaderFile(const std::string& aFilePath);

	std::string myFilePath;
	std::string myVertexShader;
	std::string myFragmentShader;

	unsigned int myVertexShaderID;
	unsigned int myFragmentShaderID;
	unsigned int myID;
};

#endif