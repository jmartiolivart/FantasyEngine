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
	
	return true;
}

update_status ModuleOpenGL::PreUpdate()
{
	return UPDATE_CONTINUE;
}

// Called every draw update
update_status ModuleOpenGL::Update()
{

	/*model =	float4x4::identity;
	view = App->camera->LookAt();
	proj = App->camera->GetProjectionMatrix();
	
	
	glUniformMatrix4fv(2, 1, GL_TRUE, &model[0][0]);
	glUniformMatrix4fv(3, 1, GL_TRUE, &view[0][0]);
	glUniformMatrix4fv(4, 1, GL_TRUE, &proj[0][0]);

	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	*/
	
	

	return UPDATE_CONTINUE;
}

update_status ModuleOpenGL::PostUpdate()
{
	//SDL_GL_SwapWindow(App->GetWindow()->window); //Change the buffer that is showing
	return UPDATE_CONTINUE;
}

// Called before quitting
bool ModuleOpenGL::CleanUp()
{
	LOG("Destroying renderer");
	glBindVertexArray(0); // Unbind VAO
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



