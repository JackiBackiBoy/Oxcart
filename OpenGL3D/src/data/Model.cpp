#include "Model.h"
#include <iostream>
#include "GL/glew.h"
#include "vendor/stb_image/stb_image.h"

Model::Model(char* aPath)
{
	LoadModel(aPath);
}

void Model::Render(Shader& aShader)
{
	for (unsigned int i = 0; i < myMeshes.size(); i++)
	{
		myMeshes[i].Render(aShader);
	}
}

unsigned int Model::TextureFromFile(const char* aPath, const std::string& aDirectory)
{
	std::string tempFileName = std::string(aPath);
	tempFileName = aDirectory + '/' + tempFileName;

	unsigned int tempTextureID;
	glGenTextures(1, &tempTextureID);

	int tempWidth;
	int tempHeight;
	int tempChannelCount;

	stbi_set_flip_vertically_on_load(true);
	unsigned char* tempData = stbi_load(tempFileName.c_str(), &tempWidth, &tempHeight, &tempChannelCount, 0);

	if (tempData)
	{
		GLenum tempFormat;

		if (tempChannelCount == 1)
		{
			tempFormat = GL_RED;
		}
		else if (tempChannelCount == 3)
		{
			tempFormat = GL_RGB;
		}
		else if (tempChannelCount == 4)
		{
			tempFormat = GL_RGBA;
		}

		glBindTexture(GL_TEXTURE_2D, tempTextureID);
		glTexImage2D(GL_TEXTURE_2D, 0, tempFormat, tempWidth, tempHeight, 0, tempFormat, GL_UNSIGNED_BYTE, tempData);
		glGenerateMipmap(GL_TEXTURE_2D);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		stbi_image_free(tempData);
	}
	else
	{
		std::cout << "Error (Assimp): " << aPath << std::endl;
		stbi_image_free(tempData);
	}

	return tempTextureID;
}

void Model::LoadModel(const std::string& aPath)
{
	Assimp::Importer tempImporter;
	const aiScene* tempScene = tempImporter.ReadFile(aPath, aiProcess_Triangulate | aiProcess_FlipUVs);

	if (!tempScene || tempScene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !tempScene->mRootNode)
	{
		std::cout << "Error (Assimp): " << tempImporter.GetErrorString() << std::endl;
		return;
	}

	myDirectory = aPath.substr(0, aPath.find_last_of('/'));

	ProcessNode(tempScene->mRootNode, tempScene);
}

void Model::ProcessNode(aiNode* aNode, const aiScene* aScene)
{
	// Process the node's meshes
	for (unsigned int i = 0; i < aNode->mNumMeshes; i++)
	{
		aiMesh* tempMesh = aScene->mMeshes[aNode->mMeshes[i]];
		myMeshes.push_back(ProcessMesh(tempMesh, aScene));
	}

	// Process the meshes of the children of the node
	for (unsigned int i = 0; i < aNode->mNumChildren; i++)
	{
		ProcessNode(aNode->mChildren[i], aScene);
	}
}

Mesh Model::ProcessMesh(aiMesh* aMesh, const aiScene* aScene)
{
	std::vector<Vertex> tempVertices;
	std::vector<unsigned int> tempIndices;
	std::vector<Texture2D> tempTextures;

	for (unsigned int i = 0; i < aMesh->mNumVertices; i++)
	{
		Vertex tempVertex;
		Vector3D tempVector3D;

		// Process vertex positions
		tempVector3D.x = aMesh->mVertices[i].x;
		tempVector3D.y = aMesh->mVertices[i].y;
		tempVector3D.z = aMesh->mVertices[i].z;
		tempVertex.position = tempVector3D;

		// Process vertex normals
		if (aMesh->HasNormals())
		{
			tempVector3D.x = aMesh->mNormals[i].x;
			tempVector3D.y = aMesh->mNormals[i].y;
			tempVector3D.z = aMesh->mNormals[i].z;
			tempVertex.normal = tempVector3D;
		}

		// Process texture coordinates
		if (aMesh->mTextureCoords[0])
		{
			Vector2D tempVector2D;
			tempVector2D.x = aMesh->mTextureCoords[0][i].x;
			tempVector2D.y = aMesh->mTextureCoords[0][i].y;
			tempVertex.textureCoords = tempVector2D;
		}
		else
		{
			tempVertex.textureCoords = { 0, 0 };
		}

		tempVertices.push_back(tempVertex);
	}

	// Process indices
	for (unsigned int i = 0; i < aMesh->mNumFaces; i++)
	{
		aiFace tempFace = aMesh->mFaces[i];

		for (unsigned int j = 0; j < tempFace.mNumIndices; j++)
		{
			tempIndices.push_back(tempFace.mIndices[j]);
		}
	}

	// Process the material
	if (aMesh->mMaterialIndex >= 0)
	{
		aiMaterial* tempMaterial = aScene->mMaterials[aMesh->mMaterialIndex];

		// Diffuse maps
		std::vector<Texture2D> tempDiffuseMaps = LoadMaterialTextures(tempMaterial, aiTextureType_DIFFUSE, "textureDiffuse");
		tempTextures.insert(tempTextures.end(), tempDiffuseMaps.begin(), tempDiffuseMaps.end());

		// Specular maps
		std::vector<Texture2D> tempSpecularMaps = LoadMaterialTextures(tempMaterial, aiTextureType_SPECULAR, "textureSpecular");
		tempTextures.insert(tempTextures.end(), tempSpecularMaps.begin(), tempSpecularMaps.end());
	}

	return Mesh(tempVertices, tempIndices, tempTextures);
}

std::vector<Texture2D> Model::LoadMaterialTextures(aiMaterial* aMaterial, aiTextureType aType, std::string aTypeName)
{
	std::vector<Texture2D> tempTextures;

	for (unsigned int i = 0; i < aMaterial->GetTextureCount(aType); i++)
	{
		aiString tempString;
		aMaterial->GetTexture(aType, i, &tempString);
		bool tempShouldSkip = false;

		for (unsigned int j = 0; j < myLoadedTextures.size(); j++)
		{
			if (std::strcmp(myLoadedTextures[j].path.data(), tempString.C_Str()) == 0)
			{
				tempTextures.push_back(myLoadedTextures[j]);
				tempShouldSkip = true;
				break;
			}
		}
		if (!tempShouldSkip)
		{
			Texture2D tempTexture;
			tempTexture.ID = TextureFromFile(tempString.C_Str(), myDirectory);
			tempTexture.type = aTypeName;
			tempTexture.path = tempString.C_Str();
			tempTextures.push_back(tempTexture);
			myLoadedTextures.push_back(tempTexture);
		}
	}

	return tempTextures;
}