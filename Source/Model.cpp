#define TINYGLTF_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "./tinygltf-2.9.3/tiny_gltf.h"

#include "Model.h"
#include "Globals.h"
#include "Mesh.h"
#include "Application.h"
#include "ModuleCamera.h"
#include "ModuleTexture.h"
#include "Log.h"

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
    LOG_TINYGLTF("Creating meshes for model...");

    setModelScaleFactor();
   
    // Apply scale and meshes
    for (const auto& srcMesh : model.meshes) {
        for (const auto& primitive : srcMesh.primitives) {
            Mesh* mesh = new Mesh;
            mesh->Load(model, srcMesh, primitive);
            mesh->LoadEBO(model, srcMesh, primitive);
            mesh->CreateVAO();
            meshes.push_back(mesh);
        }
        LOG_TINYGLTF("Mesh -> %s loaded correctly!\n\n", srcMesh.name.c_str());
    }
    LOG_TINYGLTF("Model loaded with %zu meshes", meshes.size());
    LoadMaterials();
}



void Model::LoadMaterials() {
    
    LOG("Loading materials...");

    for (const auto& material : model.materials) {
        if (material.pbrMetallicRoughness.baseColorTexture.index >= 0) {
            const auto& texture = model.textures[material.pbrMetallicRoughness.baseColorTexture.index];
            const auto& image = model.images[texture.source];
            unsigned int textureId = App->texture->Load(image.uri);
            textures.push_back(textureId);
            LOG("Material loaded texture: %s (ID: %u)", image.uri.c_str(), textureId);
        }
    }
    LOG("Total textures loaded: %zu", textures.size());
}



void Model::LoadModelFile(const char* assetFileName) {
    
    tinygltf::TinyGLTF gltfContext;
    std::string error, warning;
    
    LOG_TINYGLTF("Loading model file: %s", assetFileName);

    bool loadOk = gltfContext.LoadASCIIFromFile(&model, &error, &warning, assetFileName);
    if (!loadOk) {
        LOG_TINYGLTF("Error loading %s: %s", assetFileName, error.c_str());
    }
    else {
        LOG_TINYGLTF("Model file loaded successfully");
    }
}

std::vector<Mesh*>& Model::GetMeshes() {
    return meshes;
}

std::vector<unsigned int>& Model::GetTextures() {
    return textures;
}

void Model::setModelScaleFactor() {
    
    float maxScaleFactor = 1.0f;
    
    for (const auto& node : model.nodes) {
        if (node.scale.size() == 3) {
            float scaleX = node.scale[0];
            float scaleY = node.scale[1];
            float scaleZ = node.scale[2];
            float scaleFactor = std::max(scaleX, std::max(scaleY, scaleZ));;
            if (scaleFactor > maxScaleFactor) {
                maxScaleFactor = scaleFactor;
            }
        }
    }
    
    math::float4x4 scaleMatrix = math::float4x4::identity;
    scaleMatrix[0][0] = maxScaleFactor;
    scaleMatrix[1][1] = maxScaleFactor;
    scaleMatrix[2][2] = maxScaleFactor;
    App->camera->SetModelMatrix(scaleMatrix);

}