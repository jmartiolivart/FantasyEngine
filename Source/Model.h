#pragma once
#include <vector>
#include <string>
#include "tinygltf-2.9.3/tiny_gltf.h"

class Mesh;

class Model {
public:
    
    Model();
    Model(const char* assetFileName);
    ~Model();

    void Load(const char* assetFileName);
    void LoadMaterials();

    std::vector<Mesh*>& GetMeshes();
    std::vector<unsigned int>& GetTextures();
private:
    void LoadModelFile(const char* assetFileName);
    bool modelLoaded = false;
    struct tinygltf::Model model;
    std::vector<Mesh*> meshes;
    std::vector<unsigned int> textures;
};
