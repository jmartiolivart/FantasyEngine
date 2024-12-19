#include "ModuleOpenGL.h"
#include "Application.h"
#include "ModuleWindow.h"
#include <GL/glew.h>
#include <iostream>


ModuleOpenGL::ModuleOpenGL() {}



ModuleOpenGL::~ModuleOpenGL() {}



bool ModuleOpenGL::Init() {

    // Crear context

    context = SDL_GL_CreateContext(App->GetWindow()->window);

    if (!context) {

        LOG("Error creant context OpenGL: %s", SDL_GetError());
        return false;
    }

   // Inicialitzar GLEW

    GLenum err = glewInit();

    if (err != GLEW_OK) {

        LOG("Error inicialitzant GLEW: %s", glewGetErrorString(err));

        return false;

    }



    LOG("OpenGL version: %s", glGetString(GL_VERSION));



    // Configuració OpenGL

    glViewport(0, 0, *App->GetWindow()->window_width, *App->GetWindow()->window_height);
    glClearColor(0.3f, 0.3f, 0.5f, 1.0f);



    // Crear el triangle

    float vertices[] = {

        -0.5f, -0.5f, 0.0f,

         0.5f, -0.5f, 0.0f,

         0.0f,  0.5f, 0.0f

    };



    glGenVertexArrays(1, &VAO);

    glGenBuffers(1, &VBO);



    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);



    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);


    glEnableVertexAttribArray(0);



    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glBindVertexArray(0);



    // Crear i compilar els shaders

    const char* vertexShaderSource = R"(

        #version 330 core

        layout (location = 0) in vec3 aPos;

        void main() {

            gl_Position = vec4(aPos, 1.0);

        }

    )";



    const char* fragmentShaderSource = R"(

        #version 330 core

        out vec4 FragColor;

        void main() {

            FragColor = vec4(0.8, 0.3, 0.2, 1.0);

        }

    )";



    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);

    glShaderSource(vertexShader, 1, &vertexShaderSource, nullptr);

    glCompileShader(vertexShader);



    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

    glShaderSource(fragmentShader, 1, &fragmentShaderSource, nullptr);

    glCompileShader(fragmentShader);



    program = glCreateProgram();

    glAttachShader(program, vertexShader);

    glAttachShader(program, fragmentShader);

    glLinkProgram(program);



    glDeleteShader(vertexShader);

    glDeleteShader(fragmentShader);



    return true;

}



update_status ModuleOpenGL::PreUpdate() {

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    return UPDATE_CONTINUE;

}



update_status ModuleOpenGL::Update() {

    glUseProgram(program);

    glBindVertexArray(VAO);

    glDrawArrays(GL_TRIANGLES, 0, 3);

    glBindVertexArray(0);

    return UPDATE_CONTINUE;

}



update_status ModuleOpenGL::PostUpdate() {

    SDL_GL_SwapWindow(App->GetWindow()->window);

    return UPDATE_CONTINUE;

}



bool ModuleOpenGL::CleanUp() {

    glDeleteVertexArrays(1, &VAO);

    glDeleteBuffers(1, &VBO);

    glDeleteProgram(program);

    SDL_GL_DeleteContext(context);

    return true;

}

const float4x4& ModuleOpenGL::GetProjectionMatrix() {

    return proj; // Retorna la matriu de projecció (assegura't que està inicialitzada)

}



void ModuleOpenGL::WindowResized(unsigned int width, unsigned int height) {

    glViewport(0, 0, width, height); // Ajusta el viewport a la nova mida

}



const float4x4& ModuleOpenGL::GetViewMatrix() {

    return view; // Retorna la matriu de vista

}
