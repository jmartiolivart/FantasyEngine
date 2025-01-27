#include "ModuleModel.h"
#include "Application.h"
#include "ModuleCamera.h"
#include "Mesh.h"

ModuleModel::ModuleModel(){}

ModuleModel::~ModuleModel()
{
    if (loadedModel) {
        delete loadedModel;
        loadedModel = nullptr;
    }
}

bool ModuleModel::Init()
{
    loadedModel = new Model("Assets/BakerHouse.gltf");
    return true;
}

update_status ModuleModel::Update()
{
    viewMatrix = App->camera->LookAt();
    projMatrix = App->camera->GetProjectionMatrix();

    if (loadedModel != nullptr)
    {
        for (Mesh* mesh : loadedModel->GetMeshes())
        {
            mesh->Render();
        }
    }

    return UPDATE_CONTINUE;
}

bool ModuleModel::CleanUp()
{
    if (loadedModel)
    {
        delete loadedModel;
        loadedModel = nullptr;
    }
    return true;
}

Model* ModuleModel::GetModel()
{
    return loadedModel;
}

void ModuleModel::Load(const char* filepath)
{
    if (loadedModel) {
        delete loadedModel;
        loadedModel = nullptr;
    }
    loadedModel = new Model(filepath);

    std::string filename = filepath;
    size_t lastSlash = filename.find_last_of("/\\");
    if (lastSlash != std::string::npos) {
        filename = filename.substr(lastSlash + 1);
    }
}

void ModuleModel::LoadModel(const std::string& filePath)
{
    Load(filePath.c_str());
}

const math::float4x4& ModuleModel::GetViewMatrix() const {
    return viewMatrix;
}

const math::float4x4& ModuleModel::GetProjectionMatrix() const {
    return projMatrix;
}