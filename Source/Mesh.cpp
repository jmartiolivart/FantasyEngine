#include "Mesh.h"
#include "./tinygltf-2.9.3/tiny_gltf.h"
#include <SDL.h>
#include <GL/glew.h>
#include "math-library/Math/float3.h"
#include "ModuleOpenGL.h"
#include "Application.h"


//Pass vertex position to vbo in gpu
void Mesh::Load(const tinygltf::Model& model, const tinygltf::Mesh& mesh, const tinygltf::Primitive& primitive) {
	// Acces to vertex atribut POSITION -> if not found = primitive.attributes.end()
	const auto& itPos = primitive.attributes.find("POSITION");
	if (itPos != primitive.attributes.end()) {

		// Look accessor in string POSITION and check if it is a vec3 and float
		const tinygltf::Accessor& posAcc = model.accessors[itPos->second];
		SDL_assert(posAcc.type == TINYGLTF_TYPE_VEC3);
		SDL_assert(posAcc.componentType == GL_FLOAT);

		// Access to the vertex positions of the primitive
		const tinygltf::BufferView& posView = model.bufferViews[posAcc.bufferView];
		const tinygltf::Buffer& posBuffer = model.buffers[posView.buffer];
		const unsigned char* bufferPos = &(posBuffer.data[posAcc.byteOffset + posView.byteOffset]);

		numVertices = posAcc.count; // Initialize numVertices
		LOG("Number of vertices: %d", numVertices);

		// Calculate stride (ensure correct spacing between vertices)
		size_t stride = posView.byteStride ? posView.byteStride : sizeof(float) * 3;
		LOG("Stride size: %zu", stride);

		// List data loaded in buffer
		for (size_t i = 0; i < numVertices; ++i) {
			float x = *reinterpret_cast<const float*>(bufferPos);
			float y = *reinterpret_cast<const float*>(bufferPos + sizeof(float));
			float z = *reinterpret_cast<const float*>(bufferPos + 2 * sizeof(float));
			LOG("Vertex %zu: (%f, %f, %f)", i, x, y, z);
			bufferPos += stride; // Avancem segons el stride
		}

		// Create a VBO load data
		glGenBuffers(1, &VBO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);

		// VBO size (position + texture (if there's texture))
		size_t vboSize = sizeof(float) * 3 * numVertices; // 3 floats per posici� de v�rtex
		const auto& itTexCoord = primitive.attributes.find("TEXCOORD_0");
		if (itTexCoord != primitive.attributes.end()) {
			const tinygltf::Accessor& texCoordAcc = model.accessors[itTexCoord->second];
			vboSize += sizeof(float) * 2 * numVertices; // 2 floats per textura
		}
		LOG("VBO size: %zu bytes", vboSize);

		glBufferData(GL_ARRAY_BUFFER, vboSize, nullptr, GL_STATIC_DRAW);

		float* ptr = reinterpret_cast<float*>(glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY));

		// Trail the vertices for copy data to the buffer
		bufferPos = &(posBuffer.data[posAcc.byteOffset + posView.byteOffset]); // Reset bufferPos
		for (size_t i = 0; i < numVertices; ++i) {
			*ptr++ = *reinterpret_cast<const float*>(bufferPos);
			*ptr++ = *reinterpret_cast<const float*>(bufferPos + sizeof(float));
			*ptr++ = *reinterpret_cast<const float*>(bufferPos + 2 * sizeof(float));
			bufferPos += stride;

			LOG("Copied vertex %zu to VBO", i);
		}

		glUnmapBuffer(GL_ARRAY_BUFFER);
	}

	materialIndex = primitive.material;
}



void Mesh::LoadEBO(const tinygltf::Model& model, const tinygltf::Mesh& mesh, const tinygltf::Primitive& primitive)
{
	const tinygltf::Accessor& indAcc = model.accessors[primitive.indices];

	if (primitive.indices >= 0)
	{
		glGenBuffers(1, &EBO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * indAcc.count, nullptr, GL_STATIC_DRAW);
		unsigned int* ptr = reinterpret_cast<unsigned int*>(glMapBuffer(GL_ELEMENT_ARRAY_BUFFER, GL_WRITE_ONLY));
		const tinygltf::Accessor& indAcc = model.accessors[primitive.indices];
		const tinygltf::BufferView& indView = model.bufferViews[indAcc.bufferView];
		const unsigned char* buffer = &(model.buffers[indView.buffer].data[indAcc.byteOffset +
			indView.byteOffset]);


		switch (indAcc.componentType) {
		case TINYGLTF_PARAMETER_TYPE_UNSIGNED_INT: {
			const uint32_t* bufferInd = reinterpret_cast<const uint32_t*>(buffer);
			for (uint32_t i = 0; i < indAcc.count; ++i) {
				ptr[i] = bufferInd[i];
			}
			break;
		}
		case TINYGLTF_PARAMETER_TYPE_UNSIGNED_SHORT: {
			const unsigned short* bufferInd = reinterpret_cast<const unsigned short*>(buffer);
			for (uint32_t i = 0; i < indAcc.count; ++i) {
				ptr[i] = bufferInd[i];
			}
			break;
		}
		case TINYGLTF_PARAMETER_TYPE_UNSIGNED_BYTE: {
			const unsigned char* bufferInd = reinterpret_cast<const unsigned char*>(buffer);
			for (uint32_t i = 0; i < indAcc.count; ++i) {
				ptr[i] = bufferInd[i];
			}
			break;
		}
		default:
			SDL_assert(false); 
			break;
		}

		glUnmapBuffer(GL_ELEMENT_ARRAY_BUFFER);
		numIndices = indAcc.count;
	}
	else {
		numIndices = 0;
	}
}


void Mesh::CreateVAO()
{
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

	//glEnableVertexAttribArray(1);
	//glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*)(sizeof(float) * 3 * numVertices));

	glBindVertexArray(0);
}


void Mesh::Render()
{
	glUseProgram(App->render->getProgram());
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*)(sizeof(float) * 3 * numVertices));
	glDrawElements(GL_TRIANGLES, numIndices, GL_UNSIGNED_INT, 0);
}



void Mesh::Draw(const std::vector<unsigned>& textures)
{
	glUseProgram(App->render->getProgram());
	//glActiveTexture(GL_TEXTURE0);
	//glBindTexture(GL_TEXTURE_2D, textures[materialIndex]);
	//glUniform1i(glGetUniformLocation(App->render->getProgram(), "diffuse"), 0);
	glBindVertexArray(VAO);
	//glDrawElements(GL_TRIANGLES, numIndices, GL_UNSIGNED_INT, nullptr);
	glDrawArrays(GL_TRIANGLES, 0, numVertices);
	glBindVertexArray(0);
}


