#pragma once
#include "Module.h"
#include <string>
#include <GL/glew.h>
#include <unordered_map>

class ModuleTexture : public Module {
public:
    ModuleTexture();
    ~ModuleTexture();

    bool Init() override;
    update_status PreUpdate() override;
    update_status Update() override;
    update_status PostUpdate() override;
    bool CleanUp() override;

    unsigned int Load(const std::string& uri);

private:
    std::unordered_map<std::string, unsigned int> textureCache;

    GLint internalFormat;
    GLenum format;
    GLenum type;
};
