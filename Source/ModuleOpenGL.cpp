#include "Globals.h"
#include "Application.h"
#include "ModuleOpenGL.h"
#include "ModuleWindow.h"
#include <iostream>
#include "SDL.h"
#include <GL/glew.h>
#include "Shader.h"


#define ASSERT(x) if (!(x)) __debugbreak(); //Macro that validates contidion
#define GLCall(x) GLClearError();\
	x;\
	ASSERT(GLLogCall(#x, __FILE__, __LINE__))//Macro to not have to call the two function GLLogCall and GLLogCall every time

static void GLClearError() {
	while (glGetError() != GL_NO_ERROR);
}

static bool GLLogCall(const char* function, const char* file, int line) {
	while (GLenum error = glGetError()) {
		LOG("[OpenGL Error] (%u): %s %s:%i\n", error, file, function, line);
		return false;
	}
	return true;
}

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
	LOG("GLSL: %s\n", glGetString(GL_SHADING_LANGUAGE_VERSION));

	glEnable(GL_DEPTH_TEST); // Enable depth test
	glFrontFace(GL_CCW); // Front faces will be counter clockwise
	glDisable(GL_CULL_FACE);// Disable cull backward faces

	
	App->GetWindow()->window_width = new int;
	App->GetWindow()->window_height = new int;

	SDL_GetWindowSize(App->GetWindow()->window, App->GetWindow()->window_width, App->GetWindow()->window_height); //Get window size
	glViewport(0, 0, *(App->GetWindow()->window_width), *(App->GetWindow()->window_height)); //What part of the window will render
	glClearColor(0.3f, 0.3f, 0.5f, 1.0f); //Define the color 
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //Change the color and depth with the ones specified
	
	//Drawing Square Task
	float vertices[12] = {
	-0.5f, 0.5f, 0.0f,
	 0.5f, 0.5f, 0.0f,
	-0.5f, -0.5f, 0.0f,
	 0.5f, -0.5f, 0.0f
	};

	unsigned int indices[6] = {
		0, 1, 2,
		1, 2, 3
	};

	unsigned int VBO, EBO;

	//Affects attributs to be persistent
	GLCall(glGenVertexArrays(1, &VAO));
	GLCall(glBindVertexArray(VAO));

	//Affects buffer
	GLCall(glGenBuffers(1, &VBO));
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, VBO));
	GLCall(glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW));
	
	
	//To draw the points
	GLCall(glGenBuffers(1, &EBO));
	GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO));
	GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW));
	
	GLCall(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, 0));
	GLCall(glEnableVertexAttribArray(0));

	std::string vertexShader =  Shader::readShader("default_vertex.glsl");
	std::string fragmentShader = Shader::readShader("default_fragment.glsl");
	if (vertexShader == "ERROR_READ" || fragmentShader == "ERROR_READ") {
		LOG("Error reading shaders");
		return false;
	}

	program = Shader::CreateShader(vertexShader, fragmentShader);

	if (program == 0) {
		LOG("Error linking shaders");
		return false;
	}

	int nrAttributes;
	GLCall(glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &nrAttributes));
	LOG("Maximum nr of vertex attributes supported: %u", nrAttributes);

	
	return true;
}

update_status ModuleOpenGL::PreUpdate()
{
	
	return UPDATE_CONTINUE;
}

// Called every draw update
update_status ModuleOpenGL::Update()
{
	GLCall(glUseProgram(program));
	GLCall(glBindVertexArray(VAO));
	GLCall(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0));
	GLCall(glBindVertexArray(0)); // Unbind VAO
	SDL_GL_SwapWindow(App->GetWindow()->window); //Change the buffer that is showing
	
	return UPDATE_CONTINUE;
}

update_status ModuleOpenGL::PostUpdate()
{
	return UPDATE_CONTINUE;
}

// Called before quitting
bool ModuleOpenGL::CleanUp()
{
	LOG("Destroying renderer");

	//Destroy window
	SDL_GL_DeleteContext(context);

	return true;
}

void ModuleOpenGL::WindowResized(unsigned width, unsigned height)
{
}
