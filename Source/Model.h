#pragma once
#include <vector>
#include "Mesh.h"

class Model {

public:
	void Load(const char* assetFileName);
	void LoadMaterials(const tinygltf::Model& srcModel);

	std::vector<Mesh*>& GetMeshes();
	std::vector<unsigned int>& GetTextures();

	Model(const char* assetFileName);
	~Model();

private:

	

	std::vector<Mesh*> meshes;
	std::vector<unsigned int> textures;
	
};