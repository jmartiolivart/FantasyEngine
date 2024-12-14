#pragma once
#include "tinygltf-2.9.3/tiny_gltf.h"


class Mesh {

public:


	void Load(const tinygltf::Model& model, const tinygltf::Mesh& mesh, const tinygltf::Primitive& primitive);
	void Render();

	void LoadEBO(const tinygltf::Model& model, const tinygltf::Mesh& mesh, const tinygltf::Primitive& primitive);

	void CreateVAO();

	void Draw(const std::vector<unsigned>& textures);

private:

	unsigned int EBO;
	unsigned int VBO;
	unsigned int VAO;
	unsigned int numIndices = 0;
	unsigned int numVertices = 0;
	int materialIndex;
};
