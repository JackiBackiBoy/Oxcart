#include "Model.h"
#include <fstream>
#include <sstream>

Model::Model(const std::string& aFilePath)
{
	std::ifstream tempFileStream(aFilePath);

	std::vector<Vector3D> tempVertices;
	std::vector<Vector3D> tempNormals;

	while (!tempFileStream.eof())
	{
		std::string tempLine;
		std::getline(tempFileStream, tempLine);

		std::stringstream tempStringStream;
		tempStringStream << tempLine;

		char tempJunk; // temporary variable to store unused character

		// if a line begins with 'v' it means that it stores a vertex
		if (tempLine[0] == 'v' && tempLine[1] == ' ')
		{
			Vector3D tempVertex;

			tempStringStream >> tempJunk >> tempVertex.x >> tempVertex.y >> tempVertex.z;
			tempVertices.push_back(tempVertex);
		}
		// if a line begins with 'v' followed by 'n' means that it stores a vertex normal
		else if (tempLine[0] == 'v' && tempLine[1] == 'n')
		{
			Vector3D tempNormal;

			tempStringStream >> tempJunk >> tempJunk >> tempNormal.x >> tempNormal.y >> tempNormal.z;
			tempNormals.push_back(tempNormal);
		}
		else if (tempLine[0] == 'f')
		{
			int tempFace[3];
			int tempNormal[3];
			int tempDataElementsCount = 1;

			size_t tempPos = 0;

			// Look for "//" if the file only includes indices for the faces and normals
			while ((tempPos = tempLine.find("//")) != std::string::npos)
			{
				tempDataElementsCount = 2;

				tempLine.replace(tempPos, 2, " ");
			}

			// Clear the string stream and add the modified string
			tempStringStream.str(std::string());
			tempStringStream.clear();
			tempStringStream << tempLine;

			if (tempDataElementsCount == 1)
			{
				tempStringStream >> tempJunk >> tempFace[0] >> tempFace[1] >> tempFace[2];
			}
			else if (tempDataElementsCount == 2)
			{
				tempStringStream >> tempJunk >> tempFace[0] >> tempNormal[0] >> tempFace[1] >> tempNormal[1] >> tempFace[2] >> tempNormal[2];
			}

			Triangle tempTriangle;
			tempTriangle.vertices[0] = tempVertices[tempFace[0] - 1];
			tempTriangle.vertices[1] = tempVertices[tempFace[1] - 1];
			tempTriangle.vertices[2] = tempVertices[tempFace[2] - 1];

			if (tempDataElementsCount >= 2)
			{
				tempTriangle.normal = tempNormals[tempNormal[0] - 1]; // all vertices share the same normal, thus we only need one to determine the whole face normal
			}

			triangles.push_back(tempTriangle);
		}
	}
}

Model::~Model()
{

}