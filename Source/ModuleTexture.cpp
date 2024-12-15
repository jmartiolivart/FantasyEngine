#include "ModuleTexture.h"
#include "Globals.h"
#include <GL/glew.h>
#include <string>
#include "./DirectXTex-oct2024/DirectXTex/DirectXTex.h"


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
    // Comprovar si la textura ja està carregada
    auto it = textureCache.find(uri);
    if (it != textureCache.end()) {
        LOG("Texture already loaded: %s", uri.c_str());
        return it->second;
    }

    // Inicialitzar DirectXTex
    DirectX::TexMetadata metadata;
    DirectX::ScratchImage image;

    HRESULT hr = DirectX::LoadFromWICFile(std::wstring(uri.begin(), uri.end()).c_str(), DirectX::WIC_FLAGS_NONE, &metadata, image);
    if (FAILED(hr)) {
        LOG("Error loading texture: %s", uri.c_str());
        return 0;
    }

    GLuint textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);

    GLenum internalFormat = GL_RGBA8;
    GLenum format = GL_RGBA;
    GLenum type = GL_UNSIGNED_BYTE;

    // Assignar textura a OpenGL
    glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, metadata.width, metadata.height, 0, format, type, image.GetPixels());
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glGenerateMipmap(GL_TEXTURE_2D);

    textureCache[uri] = textureID;

    LOG("Texture loaded successfully: %s (ID: %u)", uri.c_str(), textureID);
    return textureID;
}
