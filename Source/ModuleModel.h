#pragma once
#include "Module.h"
#include "Model.h"
#include "./math-library/Math/float4x4.h"

class ModuleModel : public Module
{
public:
    ModuleModel();
    ~ModuleModel();

    bool Init() override;
    update_status Update() override;
    bool CleanUp() override;

    Model* GetModel();
    void Load(const char* filepath);
    void LoadModel(const std::string& filePath);

    const math::float4x4& GetViewMatrix() const;
    const math::float4x4& GetProjectionMatrix() const;

private:
    // 1) Punter al model 3D (per carregar/descarregar)
    Model* loadedModel = nullptr;

    // 2) Matrius float4x4 (no punters)
    math::float4x4 modelMatrix;
    math::float4x4 viewMatrix;
    math::float4x4 projMatrix;
};
