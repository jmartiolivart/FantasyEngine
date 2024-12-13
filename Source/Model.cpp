#define TINYGLTF_NO_STB_IMAGE_WRITE
#define TINYGLTF_NO_STB_IMAGE
#define TINYGLTF_NO_EXTERNAL_IMAGE
#define TINYGLTF_IMPLEMENTATION /* Only in one of the includes */
#include "./tinygltf-2.9.3/tiny_gltf.h"
#include "Model.h"
#include "Globals.h"


void Model::Load(const char* assetFileName)
{

	tinygltf::TinyGLTF gltfContext;
	tinygltf::Model model;
	std::string error, warning;
	
	bool loadOk = gltfContext.LoadASCIIFromFile(&model, &error, &warning, assetFileName);

	if (!loadOk)
	{
		LOG("Error loading %s: %s", assetFileName, error.c_str());
	}
	else {
		LOG("It's been load correctly!");
	}


	/*for (const auto& srcMesh : srcModel.meshes)
	{
		for (const auto& primitive : srcMesh.primitives)
		{
			tinygltf::Mesh* mesh = new tinygltf::Mesh;
			mesh->load(srcModel, srcMesh, primitive);
		}
	}*/


}