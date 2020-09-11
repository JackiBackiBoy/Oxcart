#include "Shader.h"
#include <fstream>
#include <sstream>
#include <GL\glew.h>

Shader::Shader(const std::string& aFilePath) : myFilePath(aFilePath)
{
	ParseShaderFile(aFilePath);

	const char* tempVertexSource = myVertexShader.c_str();
	const char* tempFragSource = myFragmentShader.c_str();

	myVertexShaderID = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(myVertexShaderID, 1, &tempVertexSource, NULL);
	glCompileShader(myVertexShaderID);

	myFragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(myFragmentShaderID, 1, &tempFragSource, NULL);
	glCompileShader(myFragmentShaderID);

	myID = glCreateProgram();
	glAttachShader(myID, myVertexShaderID);
	glAttachShader(myID, myFragmentShaderID);
	glLinkProgram(myID);

	glDeleteShader(myVertexShaderID);
	glDeleteShader(myFragmentShaderID);
}

Shader::~Shader()
{

}

void Shader::ParseShaderFile(const std::string& aFilePath)
{
	std::ifstream tempFileStream(aFilePath);
	std::stringstream tempStringStream[2]; // stores the vertex and fragment shader
	std::string tempLine;

	int tempCurrentShaderType = -1; // -1 means "None"; 0 means "Vertex Shader"; 1 means "Fragment Shader"

	// Go through each line of the shader code and separate it into vertex and fragment shader code
	while (std::getline(tempFileStream, tempLine))
	{
		if (tempLine.find("#shader") != std::string::npos)
		{
			if (tempLine.find("vertex") != std::string::npos)
			{
				tempCurrentShaderType = 0; // set current shader type to vertex
			}
			else if (tempLine.find("fragment") != std::string::npos)
			{
				tempCurrentShaderType = 1;
			}
		}
		else
		{
			tempStringStream[tempCurrentShaderType] << tempLine << '\n';
		}
	}

	myVertexShader = tempStringStream[0].str();
	myFragmentShader = tempStringStream[1].str();
}