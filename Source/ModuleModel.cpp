#include "ModuleModel.h"
#include "Application.h"

ModuleModel::ModuleModel() : model(nullptr) {}

ModuleModel::~ModuleModel() {
    if (model) delete model;
}

bool ModuleModel::Init() {
    model = new Model("Assets/BoxTextured.gltf");
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
