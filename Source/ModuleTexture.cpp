#define _CRT_SECURE_NO_WARNINGS
#include "ModuleTexture.h"
#include "./DirectXTex-oct2024/DirectXTex/DirectXTex.h"
#include <GL/glew.h>
#include <string>
#include "Shader.h"
#include "Application.h"
#include "ModuleCamera.h"
#include "Mesh.h"
#include "Model.h"
#include "ModuleOpenGL.h"

bool ModuleTexture::Init()
{

	return true;
}

update_status ModuleTexture::PreUpdate()
{

	
	return UPDATE_CONTINUE;
}

update_status ModuleTexture::Update()
{
	
	return UPDATE_CONTINUE;
}

update_status ModuleTexture::PostUpdate()
{
	return UPDATE_CONTINUE;
}

bool ModuleTexture::CleanUp()
{
	
	return true;
}

unsigned int ModuleTexture::Load(const std::string& uri) {

	// Convert URI to unicode
	size_t len = uri.length() + 1;
	wchar_t* wstr = new wchar_t[len];
	std::mbstowcs(wstr, uri.c_str(), len);
	
	HRESULT hr = DirectX::LoadFromDDSFile(wstr, DirectX::DDS_FLAGS::DDS_FLAGS_NONE, &metadata, image);

	if (FAILED(hr)) {
		hr = DirectX::LoadFromTGAFile(wstr, DirectX::TGA_FLAGS::TGA_FLAGS_NONE, &metadata, image);
		if (FAILED(hr)) {
			hr = DirectX::LoadFromWICFile(wstr, DirectX::WIC_FLAGS::WIC_FLAGS_NONE, &metadata, image);
			if (FAILED(hr)) {
				LOG("Error loading texture from %s", uri.c_str());
			}
		}
	}

	if (SUCCEEDED(hr)) {
		LOG("Succeeded loading the texture");

		switch (metadata.format)
		{
		case DXGI_FORMAT_R8G8B8A8_UNORM_SRGB:
		case DXGI_FORMAT_R8G8B8A8_UNORM:
			internalFormat = GL_RGBA8;
			format = GL_RGBA;
			type = GL_UNSIGNED_BYTE;
			LOG("Conversion to DXGI_FORMAT_R8G8B8A8_UNORM_SRGB | DXGI_FORMAT_R8G8B8A8_UNORM");
			break;
		case DXGI_FORMAT_B8G8R8A8_UNORM_SRGB:
		case DXGI_FORMAT_B8G8R8A8_UNORM:
			internalFormat = GL_RGBA8;
			format = GL_BGRA;
			type = GL_UNSIGNED_BYTE;
			LOG("Conversion to DXGI_FORMAT_B8G8R8A8_UNORM_SRGB | DXGI_FORMAT_B8G8R8A8_UNORM");
			break;
		case DXGI_FORMAT_B5G6R5_UNORM:
			internalFormat = GL_RGB8;
			format = GL_BGR;
			type = GL_UNSIGNED_BYTE;
			LOG("Conversion to DXGI_FORMAT_B5G6R5_UNORM");
			break;
		default:
			assert(false && "Unsupported format");
		}
	}
	delete[] wstr;

	glGenTextures(1, &textureID);

	//Configure texture parameters to tell use all the mipmaps availables
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_BASE_LEVEL, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, image.GetMetadata().mipLevels - 1);


	//Load to GPU all mipmaps
	if (image.GetMetadata().mipLevels > 0) {
		for (size_t i = 0; i < image.GetMetadata().mipLevels; ++i) {
			const DirectX::Image* mip = image.GetImage(i, 0, 0);
			glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, mip->width, mip->height, 0, format, type, mip->pixels);
		}
	}
	else
	{
		void* imData = image.GetPixels();
		glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, image.GetMetadata().width, image.GetMetadata().height, 0, format, type, imData);
		glGenerateMipmap(GL_TEXTURE_2D);
	}

	//Will load in the fragment shader where layout(binding=5)
	glActiveTexture(GL_TEXTURE5);
	glBindTexture(GL_TEXTURE_2D, textureID);


	return textureID;

}
