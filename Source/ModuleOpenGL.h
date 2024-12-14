#pragma once
#include "Module.h"
#include "./math-library/Math/float4x4.h"
#include "Model.h"

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

	const float4x4& GetModelMatrix();
	const float4x4& GetViewMatrix();
	const float4x4& GetProjectionMatrix();

	int getProgram();

	Model* GetModelLoaded() const;

private:

	void* context = nullptr;
	int nrAttributes;
	int program;
	unsigned int VAO;
	unsigned int VBO;
	unsigned int EBO;

	float4x4 model;
	float4x4 view;
	float4x4 proj;

	Model* modelLoaded;
};