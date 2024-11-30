#ifndef _MODULE_OPENGL_H_
#define _MODULE_OPENGL_H_
#pragma once
#include "Module.h"
#include "Globals.h"
#include "Camera.h"
#include "./math-library/Math/float4x4.h"




struct SDL_Texture;
struct SDL_Renderer;
struct SDL_Rect;

class ModuleOpenGL : public Module
{
public:
	ModuleOpenGL();
	~ModuleOpenGL();

	bool Init();
	update_status PreUpdate();
	update_status Update();
	update_status PostUpdate();
	bool CleanUp();
	void WindowResized(unsigned width, unsigned height);


	const float4x4& GetModelMatrix();
	const float4x4& GetViewMatrix();
	const float4x4& GetProjectionMatrix();



private:
	void* context = nullptr;
	unsigned int VAO = 0;
	int program = 0;
	Camera mainCamera;
	float4x4 model;
	float4x4 view;
	float4x4 proj;

};

#endif /* _MODULE_OPENGL_H_ */
