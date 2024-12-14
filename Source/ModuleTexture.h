#pragma once
#include "Module.h"
#include <GL/glew.h>
#include "./DirectXTex-oct2024/DirectXTex/DirectXTex.h"
#include "./math-library/Math/float4x4.h"


class ModuleTexture : public Module {


public:

	bool Init();
	bool CleanUp();


	update_status PreUpdate();
	update_status Update();
	update_status PostUpdate();

	unsigned int Load(const std::string& uri);

private:

	DirectX::ScratchImage image;
	DirectX::TexMetadata metadata;
	GLuint textureID;

	GLint internalFormat;
	GLenum format;
	GLenum type;

};