#include "Mesh.h"

void Mesh::Load(const tinygltf::Model& model, const tinygltf::Mesh& mesh, const tinygltf::Primitive& primitive)
{
	/*const auto& itPos = primitive.attributes.find("POSITION");
	if (itPos != primitive.attributes.end())
	{
		const tinygltf::Accessor& posAcc = model.accessors[itPos->second];
		SDL_assert(posAcc.type == TINYGLTF_TYPE_VEC3);
		SDL_assert(posAcc.componentType == GL_FLOAT);
		const BufferView& posView = model.bufferViews[posAcc.bufferView];
		const Buffer& posBuffer = model.buffers[posView.buffer];
		const unsigned char* bufferPos = &(posBuffer.data[posAcc.byteOffset + posView.byteOffset]);

		glGenBuffers(1, &vbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 3 * posAcc.count, bufferPos, GL_STATIC_DRAW);
	}*/
}