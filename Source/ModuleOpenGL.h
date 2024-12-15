#pragma once

#include "Module.h"
#include "./math-library/Math/float4x4.h"

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

    const math::float4x4& GetModelMatrix() const;
    const math::float4x4& GetViewMatrix() const;
    const math::float4x4& GetProjectionMatrix() const;
    int getProgram() const;

private:
    unsigned int program = 0;
    int modelLoc = -1;
    int viewLoc = -1;
    int projLoc = -1;
    int textureDiffuseLoc = -1;

    math::float4x4 model;
    math::float4x4 view;
    math::float4x4 proj;

    void* context = nullptr;

};
