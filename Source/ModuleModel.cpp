#include "ModuleModel.h"
#include "Application.h"
#include "./math-library/Math/float4x4.h"
#include "ModuleCamera.h"
ModuleModel::ModuleModel() : model(nullptr) {}

ModuleModel::~ModuleModel() {
    if (model) delete model;
}

bool ModuleModel::Init() {
    model = new Model("Assets/BakerHouse/BakerHouse.gltf");
    math::float4x4 scaleMatrix = math::float4x4::identity;
    scaleMatrix[0][0] = 100.0f;
    scaleMatrix[1][1] = 100.0f;
    scaleMatrix[2][2] = 100.0f;
    App->camera->SetModelMatrix(scaleMatrix);
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

void ModuleModel::Load(const char* filepath) {
    if (model) {
        delete model;
        model = nullptr;
    }
    model = new Model(filepath);

    // Comprova si el fitxer és BakerHouse.gltf
    std::string filename = filepath;
    size_t lastSlash = filename.find_last_of("/\\");
    if (lastSlash != std::string::npos) {
        filename = filename.substr(lastSlash + 1);
    }
    if (filename == "BakerHouse.gltf") {
        math::float4x4 scaleMatrix = math::float4x4::identity;
        scaleMatrix[0][0] = 100.0f;
        scaleMatrix[1][1] = 100.0f;
        scaleMatrix[2][2] = 100.0f;
        App->camera->SetModelMatrix(scaleMatrix);
    }
    else {
        App->camera->SetModelMatrix(math::float4x4::identity);
    }
}