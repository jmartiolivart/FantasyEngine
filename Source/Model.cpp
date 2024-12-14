#define TINYGLTF_NO_STB_IMAGE_WRITE
#define TINYGLTF_NO_STB_IMAGE
#define TINYGLTF_NO_EXTERNAL_IMAGE
#define TINYGLTF_IMPLEMENTATION
#include "./tinygltf-2.9.3/tiny_gltf.h"
#include "Model.h"
#include "Globals.h"
#include "Mesh.h"
#include "Application.h"
#include "ModuleTexture.h"


Model::Model(const char* assetFileName) {
	Load(assetFileName);
}

Model::~Model() {
	
	for (Mesh* mesh : meshes) {
		delete mesh;
	}
	meshes.clear();

	for (unsigned int textureId : textures) {
		glDeleteTextures(1, &textureId);
	}
	textures.clear();
}


void Model::Load(const char* assetFileName){

	tinygltf::TinyGLTF gltfContext;
	tinygltf::Model model;
	std::string error, warning;
	
	//Load JSON to TinyGLTF
	bool loadOk = gltfContext.LoadASCIIFromFile(&model, &error, &warning, assetFileName);

	if (!loadOk)
	{
		LOG("Error loading %s: %s", assetFileName, error.c_str());
	}
	
	LOG("Model meshes %d", model.meshes.size());
	//Pass all the meshes of this model and primitives of this meshes to Mesh class
	for (const auto& srcMesh : model.meshes)
	{
		LOG("Another mesh");
		for (const auto& primitive : srcMesh.primitives)
		{
			LOG("Another primitive");
			Mesh* mesh = new Mesh;
			mesh->Load(model, srcMesh, primitive);
			//mesh->LoadEBO(model, srcMesh, primitive);
			mesh->CreateVAO();
			meshes.push_back(mesh);
		}
	}
}

void Model::LoadMaterials(const tinygltf::Model& srcModel){

	for (const auto& srcMaterial : srcModel.materials)
	{
		LOG("Another material");
		unsigned int textureId = 0;
		if (srcMaterial.pbrMetallicRoughness.baseColorTexture.index >= 0)
		{
			const tinygltf::Texture& texture = srcModel.textures[srcMaterial.pbrMetallicRoughness.baseColorTexture.index];
			const tinygltf::Image& image = srcModel.images[texture.source];
			textureId = (App->texture->Load(image.uri));
		}
		textures.push_back(textureId);
	}
}


std::vector<Mesh*>& Model::GetMeshes() {
	return meshes;
}

std::vector<unsigned int>& Model::GetTextures() {
	return textures;
}