#ifndef MODEL_HEADER
#define MODEL_HEADER

#include "Mesh.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

class Model
{
public:
	Model() {};
	Model(const std::string& aPath);
	~Model() {};

	void Render(Shader& aShader);

private:
	std::vector<Mesh> myMeshes;
	std::string myDirectory;
	std::vector<Texture2D> myLoadedTextures;

	unsigned int TextureFromFile(const char* aPath, const std::string& aDirectory);

	void LoadModel(const std::string& aPath);
	void ProcessNode(aiNode* aNode, const aiScene* aScene);
	Mesh ProcessMesh(aiMesh* aMesh, const aiScene* aScene);
	std::vector<Texture2D> LoadMaterialTextures(aiMaterial* aMaterial, aiTextureType aType, std::string aTypeName);
};
#endif