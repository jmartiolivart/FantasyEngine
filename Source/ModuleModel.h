#pragma once
#include "Module.h"
#include "Model.h"

class ModuleModel : public Module {
public:
    ModuleModel();
    ~ModuleModel();

    bool Init() override;
    update_status Update() override;
    bool CleanUp() override;

    Model* GetModel();

    void Load(const char* filepath);

    void LoadModel(const std::string& filePath);


private:
    Model* model;
};
