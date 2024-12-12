#pragma once

#include "Module.h"
#include <GL/glew.h>
#include "./DirectXTex-oct2024/DirectXTex/DirectXTex.h"
#include "./math-library/Math/float4x4.h"



class ModuleTexture: public Module {


public:

	bool Init();
	bool CleanUp();


	update_status PreUpdate();
	update_status Update();
	update_status PostUpdate();

	const float4x4& GetModelMatrix();
	const float4x4& GetViewMatrix();
	const float4x4& GetProjectionMatrix();

private:

	void LoadTexture();
	void SetUp();

	void* context = nullptr;
	int nrAttributes;
	int program;
	unsigned int VAO;
	unsigned int VBO;
	unsigned int EBO;

	float4x4 model;
	float4x4 view;
	float4x4 proj;

	GLuint texture_object;


	DirectX::ScratchImage image;
	DirectX::TexMetadata metadata;
	GLuint textureID;

	GLint internalFormat;
	GLenum format;
	GLenum type;


	const wchar_t* imagePath = L"C:\\Users\\jmart\\Desktop\\Màster Videojocs\\FantasyEngine\\Game\\Assets\\Baboon.ppm";

};