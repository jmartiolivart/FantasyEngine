#ifndef _MODULE_OPENGL_H_
#define _MODULE_OPENGL_H_
#pragma once
#include "Module.h"
#include "./math-library/Math/float4x4.h"

class ModuleOpenGL : public Module
{
public:

	ModuleOpenGL();
	~ModuleOpenGL();

	bool Init();
	bool CleanUp();

	update_status PreUpdate();
	update_status Update();
	update_status PostUpdate();

	void WindowResized(unsigned width, unsigned height);



private:

	void* context = nullptr;
	int nrAttributes;

	unsigned int VAO;
	unsigned int VBO;
	unsigned int EBO;

	int program;

	float4x4 model;
	float4x4 view;
	float4x4 proj;

};

#endif /* _MODULE_OPENGL_H_ */