#pragma once
#include "tinygltf-2.9.3/tiny_gltf.h"


class Mesh {

public:


private:

	void Load(const tinygltf::Model& model, const tinygltf::Mesh& mesh, const tinygltf::Primitive& primitive);
};