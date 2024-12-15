#pragma once
#include "tinygltf-2.9.3/tiny_gltf.h"
#include <vector>
#include "math-library/Math/float4x4.h"

class Mesh {
public:
    void Load(const struct tinygltf::Model& model, const struct tinygltf::Mesh& mesh, const struct tinygltf::Primitive& primitive);
    void LoadEBO(const struct tinygltf::Model& model, const struct tinygltf::Mesh& mesh, const struct tinygltf::Primitive& primitive);
    void CreateVAO();
    void Render();
    void Draw(const std::vector<unsigned>& textures);
    void SetMatrices(const float4x4& model, const float4x4& view, const float4x4& proj);
    void SetModelMatrix(const math::float4x4& transform);
    const math::float4x4& GetModelMatrix() const; // Getter p�blic



private:
    unsigned int VBO = 0;
    unsigned int EBO = 0;
    unsigned int VAO = 0;
    unsigned int numVertices = 0;
    unsigned int numIndices = 0;
    bool hasTexCoords = false;
    int materialIndex = -1;
    float4x4 modelMatrix;
    float4x4 viewMatrix;
    float4x4 projMatrix;


    bool prova = true;
};
