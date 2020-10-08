#ifndef SHADER_HEADER
#define SHADER_HEADER

#include <string>
#include <GL\glew.h>
#include "math\Matrix4x4.h"

class Shader
{
public:
	Shader() {};
	Shader(const std::string& aFilePath);
	~Shader(); 

	void SetUniform1i(const std::string& aUniformName, const int& anInt) const;

	void SetUniform1f(const std::string& aUniformName, const float& aFloat) const;
	void SetUniform3f(const std::string& aUniformName, const float& aFloat1, const float& aFloat2, const float& aFloat3) const;
	void SetUniform4f(const std::string& aUniformName, const float& aFloat1, const float& aFloat2, const float& aFloat3, const float& aFloat4) const;

	void SetUniformMatrix4x4(const std::string& aUniformName, Matrix4x4& aMatrix);

	inline void Use() const { glUseProgram(myID); }

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
	int myUniformLocation;
};

#endif