#pragma once

#include "Module.h"
#include <GL/glew.h>

class ModuleOpenGL : public Module {
public:
    ModuleOpenGL();
    ~ModuleOpenGL();

    bool Init() override;
    update_status PreUpdate() override;
    update_status Update() override;
    update_status PostUpdate() override;
    bool CleanUp() override;
    void WindowResized(unsigned width, unsigned height);
    void VsyncEnabled(bool isEnable);
    void CullingFaceEnabled(bool isEnable);

    int getProgram() const;
   
    unsigned int textureID = 0;
    GLint textureDiffuseLoc = -1; // Uniform location per a la textura

private:
    unsigned int program = 0;
    int modelLoc = -1;
    int viewLoc = -1;
    int projLoc = -1;

    void* context = nullptr;

};
