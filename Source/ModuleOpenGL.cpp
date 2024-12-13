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

	Model myModel;
	myModel.Load(R"(C:\Users\jmart\Desktop\MasterVideojocs\FantasyEngine\Game\Assets\triangleGLTF.gltf)");

	return true;
}

update_status ModuleOpenGL::PreUpdate()
{
	return UPDATE_CONTINUE;
}

// Called every draw update
update_status ModuleOpenGL::Update()
{

	


	return UPDATE_CONTINUE;
}

update_status ModuleOpenGL::PostUpdate()
{
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


