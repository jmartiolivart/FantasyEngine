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

    // Crear context OpenGL
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

    glUseProgram(this->program);

    // Obtenir ubicació del uniform "texture_diffuse"
    this->textureDiffuseLoc = glGetUniformLocation(this->program, "texture_diffuse");
    if (this->textureDiffuseLoc == -1) {
        LOG("Could not find uniform location for 'texture_diffuse'");
    }

    // Configuració del quad
    float quadVertices[] = {
        // Posició        // Coordenades de textura
        -1.0f, -1.0f, 0.0f,  0.0f, 0.0f, // Inferior esquerre
         1.0f, -1.0f, 0.0f,  1.0f, 0.0f, // Inferior dret
        -1.0f,  1.0f, 0.0f,  0.0f, 1.0f, // Superior esquerre
         1.0f,  1.0f, 0.0f,  1.0f, 1.0f  // Superior dret
    };

    unsigned int quadIndices[] = {
        0, 1, 2, // Triangle 1
        1, 3, 2  // Triangle 2
    };

    // Crear VAO, VBO i EBO
    glGenVertexArrays(1, &quadVAO);
    glGenBuffers(1, &quadVBO);
    glGenBuffers(1, &quadEBO);

    glBindVertexArray(quadVAO);

    // Configurar VBO
    glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), quadVertices, GL_STATIC_DRAW);

    // Configurar EBO
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, quadEBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(quadIndices), quadIndices, GL_STATIC_DRAW);

    // Atribut de posició
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);

    // Atribut de coordenades de textura
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));

    glBindVertexArray(0);

    // Carregar la textura
    LOG("Texture ID loaded: %u", textureID);

    LOG("OpenGL initialized successfully.");
    return true;
}




update_status ModuleOpenGL::PreUpdate() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    return UPDATE_CONTINUE;
}

update_status ModuleOpenGL::Update() {
    
    // Model to load
    Model* model = App->model->GetModel();

    // Assing view and projection matrix
    this->view = App->camera->LookAt();
    this->proj = App->camera->GetProjectionMatrix();

    // Render the meshes
    for (Mesh* mesh : model->GetMeshes()) {
        mesh->SetMatrices(math::float4x4::identity, this->view, this->proj);
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

    if (quadVAO) {
        glDeleteVertexArrays(1, &quadVAO);
        glDeleteBuffers(1, &quadVBO);
        glDeleteBuffers(1, &quadEBO);
    }

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
