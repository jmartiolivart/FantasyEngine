#define TINYGLTF_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "./tinygltf-2.9.3/tiny_gltf.h"

#include "Model.h"
#include "Globals.h"
#include "Mesh.h"
#include "Application.h"
#include "ModuleTexture.h"

Model::Model() {}

Model::Model(const char* assetFileName) {
    Load(assetFileName);
}

Model::~Model() {
    for (Mesh* mesh : meshes) {
        delete mesh;
    }
    meshes.clear();
}

void Model::Load(const char* assetFileName) {
    LoadModelFile(assetFileName);
    LOG("Creating meshes for model...");

    for (const auto& srcMesh : model.meshes) {
        for (const auto& primitive : srcMesh.primitives) {
            Mesh* mesh = new Mesh;
            mesh->Load(model, srcMesh, primitive);
            mesh->LoadEBO(model, srcMesh, primitive);
            mesh->CreateVAO();
            meshes.push_back(mesh);
        }
    }

    LOG("Model loaded with %zu meshes", meshes.size());
}

void Model::LoadMaterials() {
    LOG("Loading materials...");
    for (const auto& material : model.materials) {
        if (material.pbrMetallicRoughness.baseColorTexture.index >= 0) {
            const auto& texture = model.textures[material.pbrMetallicRoughness.baseColorTexture.index];
            const auto& image = model.images[texture.source];
            unsigned int textureId = App->texture->Load(image.uri); 
            textures.push_back(textureId);
        }
    }

}

void Model::LoadModelFile(const char* assetFileName) {
    tinygltf::TinyGLTF gltfContext;
    std::string error, warning;
    LOG("Loading model file: %s", assetFileName);

    bool loadOk = gltfContext.LoadASCIIFromFile(&model, &error, &warning, assetFileName);
    if (!loadOk) {
        LOG("Error loading %s: %s", assetFileName, error.c_str());
    }
    else {
        LOG("Model file loaded successfully");
    }
}

std::vector<Mesh*>& Model::GetMeshes() {
    return meshes;
}

std::vector<unsigned int>& Model::GetTextures() {
    return textures;
}