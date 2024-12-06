#include "Globals.h"
#include "Application.h"
#include "ModuleOpenGL.h"
#include "ModuleWindow.h"
#include "SDL.h"
#include <GL/glew.h>
#include "Shader.h"
#include "ModuleCamera.h"
#include "./math-library/Math/MathAll.h"


ModuleOpenGL::ModuleOpenGL()
{
}

// Destructor
ModuleOpenGL::~ModuleOpenGL()
{
}

// Called before render is available
bool ModuleOpenGL::Init()
{
	LOG("Creating Renderer context");
	
	context = SDL_GL_CreateContext(App->GetWindow()->window);
	if (context == NULL) {
		LOG("Error creating OpenGL context: %s\n", SDL_GetError());
		return false;
	}
	
	GLenum err = glewInit();
	if (GLEW_OK != err)
	{
		fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
	}

	//Some info about the GPU thanks to glew
	LOG("Using Glew %s", glewGetString(GLEW_VERSION));
	LOG("Vendor: %s", glGetString(GL_VENDOR));
	LOG("Renderer: %s", glGetString(GL_RENDERER));
	LOG("OpenGL version supported %s", glGetString(GL_VERSION));
	LOG("GLSL: %s", glGetString(GL_SHADING_LANGUAGE_VERSION));
	glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &nrAttributes);
	LOG("Maximum nr of vertex attributes supported: %u\n", nrAttributes);


	glEnable(GL_DEPTH_TEST); // Enable depth test
	glFrontFace(GL_CCW); // Front faces will be counter clockwise
	glDisable(GL_CULL_FACE);// Disable cull backward faces


	glClearColor(0.3f, 0.3f, 0.5f, 1.0f); //Define the color 
	glViewport(0, 0, *(App->GetWindow()->window_width), *(App->GetWindow()->window_height)); //What part of the window will render


	//Drawing Triangle
	float vertices[9] = {
	-0.5f, -0.5f, 0.0f,
	 0.5f, -0.5f, 0.0f,
	 0.0f,  0.5f, 0.0f,
	};

	//Affects attributs to be persistent
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, (void*)0);
	glEnableVertexAttribArray(0);
	

	std::string vertexShader =  Shader::readShader("default_vertex.glsl");
	std::string fragmentShader = Shader::readShader("default_fragment.glsl");

	program = Shader::CreateShader(vertexShader, fragmentShader);

	if (program == 0) {
		LOG("Error linking shaders");
		return false;
	}
	return true;
}

update_status ModuleOpenGL::PreUpdate()
{
	//SDL_GetWindowSize(App->GetWindow()->window, App->GetWindow()->window_width, App->GetWindow()->window_height);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //Change the color and depth with the ones specified

	return UPDATE_CONTINUE;
}

// Called every draw update
update_status ModuleOpenGL::Update()
{
	glUseProgram(program);

	model = float4x4::identity;
	view = App->camera->LookAt();
	proj = App->camera->GetProjectionMatrix();
	
		
	glUniformMatrix4fv(0, 1, GL_TRUE, &model[0][0]);
	glUniformMatrix4fv(1, 1, GL_TRUE, &view[0][0]);
	glUniformMatrix4fv(2, 1, GL_TRUE, &proj[0][0]);


	glBindVertexArray(VAO);
	glDrawArrays(GL_TRIANGLES, 0, 3);
	glBindVertexArray(0); // Unbind VAO
	return UPDATE_CONTINUE;
}

update_status ModuleOpenGL::PostUpdate()
{
	SDL_GL_SwapWindow(App->GetWindow()->window); //Change the buffer that is showing
	return UPDATE_CONTINUE;
}

// Called before quitting
bool ModuleOpenGL::CleanUp()
{
	LOG("Destroying renderer");
	glDeleteBuffers(1, &VBO);
	glDeleteVertexArrays(1, &VAO);
	glDeleteProgram(program);
	//Destroy window
	SDL_GL_DeleteContext(context);

	return true;
}

void ModuleOpenGL::WindowResized(unsigned width, unsigned height)
{
	glViewport(0, 0, width, height);
	float newAspectRatio = (float)width / height;
	App->camera->SetAspectRatio(newAspectRatio); 
}

const float4x4& ModuleOpenGL::GetModelMatrix() {
	return model;
}

const float4x4& ModuleOpenGL::GetViewMatrix() {
	return view;
}

const float4x4& ModuleOpenGL::GetProjectionMatrix() {
	return proj;
}

