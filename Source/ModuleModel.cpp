#include "ModuleModel.h"
#include "Application.h"
#include "./math-library/Math/float4x4.h"
#include "ModuleCamera.h"

ModuleModel::ModuleModel() : model(nullptr) {}

ModuleModel::~ModuleModel() {
    if (model) delete model;
}

bool ModuleModel::Init() {
    model = new Model("Assets/BakerHouse.gltf");
    return true;
}

update_status ModuleModel::Update() {
    return UPDATE_CONTINUE;
}

bool ModuleModel::CleanUp() {
    if (model) {
        delete model;
        model = nullptr;
    }
    return true;
}

Model* ModuleModel::GetModel() {
    return model;
}

//Function to load a new module when Drag and Drop
void ModuleModel::Load(const char* filepath) {

    if (model) {
        delete model;
        model = nullptr;
    }
    model = new Model(filepath);

    std::string filename = filepath;
    size_t lastSlash = filename.find_last_of("/\\");
    if (lastSlash != std::string::npos) {
        filename = filename.substr(lastSlash + 1);
    }
}