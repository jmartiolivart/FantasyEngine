#include "Mesh.h"
#include "Globals.h"
#include "./tinygltf-2.9.3/tiny_gltf.h"
#include <SDL.h>
#include <GL/glew.h>
#include "math-library/Math/float3.h"
#include "Application.h"
#include "Model.h"
#include "ModuleOpenGL.h"
#include "ModuleCamera.h"
#include "ModuleModel.h"
#include "Log.h"

void Mesh::Load(const tinygltf::Model& model, const tinygltf::Mesh& mesh, const tinygltf::Primitive& primitive) {
    
    LOG_TINYGLTF("Loading mesh...");

    const unsigned char* bufferPos = nullptr;
    size_t posStride = 0;

    // Load positions
    LoadPositions(model, primitive, bufferPos, posStride);
    if (bufferPos == nullptr) {
        return; // No positions, abort loading
    }

    const unsigned char* bufferTexCoord = nullptr;
    size_t texStride = 0;

    // Load textures
    LoadTextures(model, primitive, bufferTexCoord, texStride);

    /*** Creation and load VBO ***/
    // Calculating the VBO size for all attributes loaded
    size_t vboSize = sizeof(float) * 3 * numVertices + (hasTexCoords ? sizeof(float) * 2 * numVertices : 0);
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    // Create the buffer in the GPU
    glBufferData(GL_ARRAY_BUFFER, vboSize, nullptr, GL_STATIC_DRAW);

    // Buffer ready in CPU
    std::vector<float> bufferData;
    bufferData.reserve(numVertices * 3 + (hasTexCoords ? numVertices * 2 : 0));

    const unsigned char* currentPos = bufferPos;
    const unsigned char* currentTex = bufferTexCoord;

    for (size_t i = 0; i < numVertices; ++i) {

        // Positions
        float px = *(float*)(currentPos);
        float py = *(float*)(currentPos + sizeof(float));
        float pz = *(float*)(currentPos + 2 * sizeof(float));
        bufferData.push_back(px);
        bufferData.push_back(py);
        bufferData.push_back(pz);
        currentPos += posStride;

        // Texture coordinates
        if (hasTexCoords) {
            float u = *(float*)(currentTex);
            float v = *(float*)(currentTex + sizeof(float));
            v = 1.0f - v; // Invertir l'eix Y
            bufferData.push_back(u);
            bufferData.push_back(v);
            currentTex += texStride;
        }
    }

    // Send data to GPU
    glBufferSubData(GL_ARRAY_BUFFER, 0, bufferData.size() * sizeof(float), bufferData.data());

    materialIndex = primitive.material;
    LOG_TINYGLTF("Mesh loaded. Vertices: %u, HasTexCoords: %d, MaterialIndex: %d", numVertices, hasTexCoords ? 1 : 0, materialIndex);
}

/*** Loading mesh positions ***/
void Mesh::LoadPositions(const tinygltf::Model& model, const tinygltf::Primitive& primitive, const unsigned char*& bufferPos, size_t& posStride) {
    
    const auto itPos = primitive.attributes.find("POSITION");
    if (itPos == primitive.attributes.end()) {
        LOG_TINYGLTF("Warning: No position attribute found");
        return;
    }
    const tinygltf::Accessor& posAcc = model.accessors[itPos->second];
    SDL_assert(posAcc.type == TINYGLTF_TYPE_VEC3 && posAcc.componentType == GL_FLOAT);

    const tinygltf::BufferView& posView = model.bufferViews[posAcc.bufferView];
    const tinygltf::Buffer& posBuffer = model.buffers[posView.buffer];
    bufferPos = &posBuffer.data[posAcc.byteOffset + posView.byteOffset];

    numVertices = (unsigned int)posAcc.count;
    posStride = posView.byteStride ? posView.byteStride : sizeof(float) * 3;
}

/*** Loading mesh textures ***/
void Mesh::LoadTextures(const tinygltf::Model& model, const tinygltf::Primitive& primitive, const unsigned char*& bufferTexCoord, size_t& texStride) {
    
    // Initialize a null and 0 in case the mesh doesn't have textures
    bufferTexCoord = nullptr;
    texStride = 0;
    hasTexCoords = false;

    const auto itTex = primitive.attributes.find("TEXCOORD_0");
    if (itTex != primitive.attributes.end()) {
        const tinygltf::Accessor& texAcc = model.accessors[itTex->second];
        SDL_assert(texAcc.type == TINYGLTF_TYPE_VEC2 && texAcc.componentType == GL_FLOAT);

        const tinygltf::BufferView& texView = model.bufferViews[texAcc.bufferView];
        const tinygltf::Buffer& texBuffer = model.buffers[texView.buffer];
        bufferTexCoord = &texBuffer.data[texAcc.byteOffset + texView.byteOffset];

        // Add stride if exists. Default is 0
        texStride = texView.byteStride ? texView.byteStride : sizeof(float) * 2;
        hasTexCoords = true;
    }
    else {
        LOG_TINYGLTF("Warning: No texture coordinates found");
    }
}

void Mesh::LoadEBO(const tinygltf::Model& model, const tinygltf::Mesh& mesh, const tinygltf::Primitive& primitive) {
    
    if (primitive.indices < 0) {
        numIndices = 0;
        return;
    }

    LOG("Loading EBO...");
    const tinygltf::Accessor& indAcc = model.accessors[primitive.indices];
    const tinygltf::BufferView& indView = model.bufferViews[indAcc.bufferView];
    const unsigned char* buffer = &model.buffers[indView.buffer].data[indAcc.byteOffset + indView.byteOffset];

    numIndices = (unsigned int)indAcc.count;

    // Prepare indices in CPU memory
    std::vector<unsigned int> indices(numIndices);

    // Types of components calculation: UNSIGNED INT, UNSIGNED SHORT or UNSIGNED BYTE
    switch (indAcc.componentType) {
    case TINYGLTF_PARAMETER_TYPE_UNSIGNED_INT: {
        const  unsigned int* src = (const  unsigned int*)buffer;
        for (size_t i = 0; i < numIndices; ++i) {
            indices[i] = src[i];
        }
        break;
    }
    case TINYGLTF_PARAMETER_TYPE_UNSIGNED_SHORT: {
        const unsigned short* src = (const unsigned short*)buffer;
        for (size_t i = 0; i < numIndices; ++i) {
            indices[i] = src[i];
        }
        break;
    }
    case TINYGLTF_PARAMETER_TYPE_UNSIGNED_BYTE: {
        const unsigned char* src = (const unsigned char*)buffer;
        for (size_t i = 0; i < numIndices; ++i) {
            indices[i] = src[i];
        }
        break;
    }
    default:
        LOG_TINYGLTF("Unsupported index type");
        SDL_assert(false);
    }

    // Create and upload EBO
    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * numIndices, indices.data(), GL_STATIC_DRAW);

    LOG_TINYGLTF("EBO loaded with %u indices", numIndices);
}


void Mesh::CreateVAO() {
    
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    if (hasTexCoords) {
        // position (3 floats), texcoord (2 floats)
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);

        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    }

    else {
        // position only (3 floats)
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    }

    if (EBO) {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    }

    glBindVertexArray(0);
    LOG_TINYGLTF("VAO created with ID: %u", VAO);
}

void Mesh::Render() {

    glUseProgram(App->render->getProgram());
    //LOG("Shader program ID: %u", App->render->getProgram());

    glUniformMatrix4fv(2, 1, GL_TRUE, &App->camera->GetModelMatrix()[0][0]); //MODEL
    glUniformMatrix4fv(3, 1, GL_TRUE, &App->camera->LookAt()[0][0]); //VIEW
    glUniformMatrix4fv(4, 1, GL_TRUE, &App->camera->GetProjectionMatrix()[0][0]); //PROJECTION

    //LOG("Rendering mesh with materialIndex: %d", materialIndex);

    // Enllaçar la textura si n'hi ha
    if (materialIndex >= 0 && materialIndex < (int)App->model->GetModel()->GetTextures().size()) {
        unsigned texId = App->model->GetModel()->GetTextures()[materialIndex];
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texId);

        int textureLoc = glGetUniformLocation(App->render->getProgram(), "texture_diffuse");
        if (textureLoc != -1) {
            glUniform1i(textureLoc, 0);
            //LOG("Bound texture ID: %u to texture_diffuse", texId);
        }
        else {
            //LOG("Could not find uniform 'texture_diffuse' in shader");
        }
    }
    else {
        LOG("No texture bound for materialIndex: %d", materialIndex);
    }

    //LOG("Mesh VAO: %u, Vertices: %u, Indices: %u", VAO, numVertices, numIndices);

    glBindVertexArray(VAO);
    //LOG("Bound VAO: %u", VAO);

    if (numIndices > 0) {
        //LOG("Drawing elements: %d indices", numIndices);
        glDrawElements(GL_TRIANGLES, numIndices, GL_UNSIGNED_INT, nullptr);
    }
    else {
        //LOG("Drawing arrays: %d vertices", numVertices);
        glDrawArrays(GL_TRIANGLES, 0, numVertices);
    }

    glBindVertexArray(0);
    glUseProgram(0);
}

void Mesh::SetMatrices(const float4x4& model, const float4x4& view, const float4x4& proj) {
    modelMatrix = model;
    viewMatrix = view;
    projMatrix = proj;
    LOG("Mesh ModelMatrix position: %f, %f, %f", modelMatrix[3][0], modelMatrix[3][1], modelMatrix[3][2]);

}
