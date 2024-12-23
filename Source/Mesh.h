#pragma once
#include "tinygltf-2.9.3/tiny_gltf.h"
#include <vector>
#include "math-library/Math/float4x4.h"

class Mesh {
public:
    void Load(const struct tinygltf::Model& model, const struct tinygltf::Mesh& mesh, const struct tinygltf::Primitive& primitive);
    void LoadPositions(const tinygltf::Model& model, const tinygltf::Primitive& primitive, const unsigned char*& bufferPos, size_t& posStride);
    void LoadTextures(const tinygltf::Model& model, const tinygltf::Primitive& primitive, const unsigned char*& bufferTexCoord, size_t& texStride);
    void LoadEBO(const struct tinygltf::Model& model, const struct tinygltf::Mesh& mesh, const struct tinygltf::Primitive& primitive);
    void CreateVAO();
    void Render();
    void SetMatrices(const float4x4& model, const float4x4& view, const float4x4& proj);



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

    

};
