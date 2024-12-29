#include "ModuleOpenGL.h"
#include "Application.h"
#include "ModuleWindow.h"
#include "ModuleCamera.h"
#include "Shader.h"
#include "Globals.h"
#include <GL/glew.h>
#include <SDL.h>
#include <string>
#include "Mesh.h"
#include "./math-library/Math/float4x4.h"
#include "ModuleTexture.h"

ModuleOpenGL::ModuleOpenGL() {}
ModuleOpenGL::~ModuleOpenGL() {}


bool ModuleOpenGL::Init() {
    LOG("Initializing OpenGL...");

    context = SDL_GL_CreateContext(App->GetWindow()->window);
    if (context == NULL) {
        LOG("Error creating OpenGL context: %s\n", SDL_GetError());
        return false;
    }

    GLenum err = glewInit();
    if (err != GLEW_OK) {
        LOG("Error initializing GLEW: %s", glewGetErrorString(err));
        return false;
    }

    // Configuració OpenGL
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);

    // Crear i carregar shaders
    std::string vertexShader = Shader::readShader("default_vertex.glsl");
    std::string fragmentShader = Shader::readShader("default_fragment.glsl");
    this->program = Shader::CreateShader(vertexShader, fragmentShader);

    if (this->program == 0) {
        LOG("Error loading shaders");
        return false;
    }


    LOG("OpenGL initialized successfully.");
    return true;
}




update_status ModuleOpenGL::PreUpdate() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    return UPDATE_CONTINUE;
}

update_status ModuleOpenGL::Update() {

    Model* model = App->model->GetModel();
    this->view = App->camera->LookAt();
    this->proj = App->camera->GetProjectionMatrix();

    //Render the meshes
    for (Mesh* mesh : model->GetMeshes()) {
        mesh->Render();
    }

    return UPDATE_CONTINUE;
}




update_status ModuleOpenGL::PostUpdate() {
    SDL_GL_SwapWindow(App->GetWindow()->window);
    return UPDATE_CONTINUE;
}

bool ModuleOpenGL::CleanUp() {
    LOG("Cleaning up OpenGL module...");

    if (this->program) {
        glDeleteProgram(this->program);
    }

    return true;
}



void ModuleOpenGL::WindowResized(unsigned width, unsigned height) {
    glViewport(0, 0, width, height);
    float newAspectRatio = static_cast<float>(width) / static_cast<float>(height);
    App->camera->SetAspectRatio(newAspectRatio);
}

void ModuleOpenGL::VsyncEnabled(bool isEnabled)
{
    isEnabled ? glEnable(VSYNC) : glDisable(VSYNC);
}

void ModuleOpenGL::CullingFaceEnabled(bool isEnabled) {
    
    if(isEnabled) {
        glEnable(GL_CULL_FACE);
        glCullFace(GL_FRONT_AND_BACK);
    }else {
        glDisable(GL_CULL_FACE);
    }
}

const math::float4x4& ModuleOpenGL::GetModelMatrix() const {
    return this->model;
}

const math::float4x4& ModuleOpenGL::GetViewMatrix() const {
    return this->view;
}

const math::float4x4& ModuleOpenGL::GetProjectionMatrix() const {
    return this->proj;
}

int ModuleOpenGL::getProgram() const {
    return this->program;
}