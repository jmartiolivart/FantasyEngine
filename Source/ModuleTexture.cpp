#define _CRT_SECURE_NO_WARNINGS
#include "ModuleTexture.h"
#include "Globals.h"
#include <GL/glew.h>
#include <string>
#include "./DirectXTex-oct2024/DirectXTex/DirectXTex.h"
#include "tinygltf-2.9.3/stb_image.h"


ModuleTexture::ModuleTexture() {}
ModuleTexture::~ModuleTexture() {}

bool ModuleTexture::Init() {
    LOG("Initializing texture module...");
    return true;
}

update_status ModuleTexture::PreUpdate() {
    return UPDATE_CONTINUE;
}

update_status ModuleTexture::Update() {
    return UPDATE_CONTINUE;
}

update_status ModuleTexture::PostUpdate() {
    return UPDATE_CONTINUE;
}

bool ModuleTexture::CleanUp() {
    LOG("Cleaning up texture module...");
    for (auto& texture : textureCache) {
        glDeleteTextures(1, &texture.second);
    }
    textureCache.clear();
    return true;
}

unsigned int ModuleTexture::Load(const std::string& uri) {
    
    std::string basePath = "Assets/";
    std::string fullPath = basePath + uri;

    auto it = textureCache.find(fullPath);
    if (it != textureCache.end()) {
        LOG("Texture already loaded: %s", fullPath.c_str());
        return it->second;
    }

    unsigned int textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);

    int width, height, nrChannels;
    stbi_set_flip_vertically_on_load(true);
    unsigned char* data = stbi_load(fullPath.c_str(), &width, &height, &nrChannels, 0);
    if (data) {
        GLenum format = (nrChannels == 4) ? GL_RGBA : GL_RGB;
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
        LOG("Loaded texture: %s (ID: %u)", fullPath.c_str(), textureID);
    }
    else {
        LOG("Failed to load texture: %s", fullPath.c_str());
    }
    stbi_image_free(data);

  
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    textureCache[fullPath] = textureID;
    return textureID;
}