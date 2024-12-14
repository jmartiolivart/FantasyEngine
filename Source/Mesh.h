#pragma once
#include "tinygltf-2.9.3/tiny_gltf.h"
#include "./math-library/Math/float4x4.h"

class Mesh {

public:


	void Load(const tinygltf::Model& model, const tinygltf::Mesh& mesh, const tinygltf::Primitive& primitive);
	void Render();

	void LoadEBO(const tinygltf::Model& model, const tinygltf::Mesh& mesh, const tinygltf::Primitive& primitive);

	void CreateVAO();

	void Draw(const std::vector<unsigned>& textures);

	void SetMatrices(const float4x4& model, const float4x4& view, const float4x4& proj);

private:

	unsigned int EBO;
	unsigned int VBO;
	unsigned int VAO;
	unsigned int numIndices = 0;
	unsigned int numVertices = 0;
	int materialIndex;


	float4x4 modelMatrix;
	float4x4 viewMatrix;
	float4x4 projMatrix;

};
