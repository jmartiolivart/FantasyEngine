#include "Globals.h"
#include "Application.h"
#include "ModuleOpenGL.h"
#include "ModuleWindow.h"
#include "SDL.h"
#include <GL/glew.h>
#include "Shader.h"
#include "ModuleCamera.h"
#include "./math-library/Math/MathAll.h"
#include "Model.h"
#include "Application.h"
#include "ModuleTexture.h"
#include "Mesh.h"

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
	}

	//Retrive functions avaliable from the OpenGL
	GLenum err = glewInit();
	if (GLEW_OK != err)
	{
		fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
	}

	//Some info about the GPU usign glew
	LOG("Using Glew %s", glewGetString(GLEW_VERSION));
	LOG("Vendor: %s", glGetString(GL_VENDOR));
	LOG("Renderer: %s", glGetString(GL_RENDERER));
	LOG("OpenGL version supported %s", glGetString(GL_VERSION));
	LOG("GLSL: %s", glGetString(GL_SHADING_LANGUAGE_VERSION));
	glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &nrAttributes);
	LOG("Maximum nr of vertex attributes supported: %u\n", nrAttributes);

	const char* assetPath = R"(C:\Users\jmart\Desktop\MasterVideojocs\FantasyEngine\Game\Assets\TriangleWithoutIndices.gltf)";
	modelLoaded = new Model(assetPath);

	glFrontFace(GL_CCW);	 // Front faces will be counter clockwise
	glEnable(GL_DEPTH_TEST); // Enable depth test
	glDisable(GL_CULL_FACE); // Disable cull backward faces


	glClearColor(0.7f, 0.7f, 0.7f, 1.0f); //Define the color of background 
	glViewport(0, 0, *(App->GetWindow()->window_width), *(App->GetWindow()->window_height)); //What part of the window will render

	//Shaders read and apply
	std::string vertexShader = Shader::readShader("default_vertex.glsl");
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
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	return UPDATE_CONTINUE;
}

// Called every draw update
update_status ModuleOpenGL::Update()
{

	model = float4x4::identity;
	view = App->camera->LookAt();
	proj = App->camera->GetProjectionMatrix();

	//glUniformMatrix4fv(2, 1, GL_TRUE, &model[0][0]);
	//glUniformMatrix4fv(3, 1, GL_TRUE, &view[0][0]);
	//glUniformMatrix4fv(4, 1, GL_TRUE, &proj[0][0]);

	//glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

	/*for (Mesh* mesh : modelLoaded->GetMeshes()) {
		mesh->Render();
	}*/
	for (Mesh* mesh : App->render->GetModelLoaded()->GetMeshes()) {
		mesh->Draw(App->render->GetModelLoaded()->GetTextures());
	}

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

int ModuleOpenGL::getProgram()
{
	return program;
}


Model* ModuleOpenGL::GetModelLoaded() const {
	return modelLoaded;
}