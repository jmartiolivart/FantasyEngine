#include "ModuleTexture.h"
#include "./DirectXTex-oct2024/DirectXTex/DirectXTex.h"
#include <GL/glew.h>
#include <string>
#include "Shader.h"
#include "Application.h"
#include "ModuleCamera.h"

bool ModuleTexture::Init()
{
	SetUp();
	LoadTexture();


	/***********TEXTURE SECTION******************/
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
	

	/***********TEXTURE SECTION END******************/


	/***********SHADER VBO VAO SECTION******************/

	
	float positions[] = {
    -0.5f,  0.5f, 0.0f, 
     0.5f,  0.5f, 0.0f,
    -0.5f, -0.5f, 0.0f,
     0.5f, -0.5f, 0.0f,
	};

	float texCoords[] = {
	0.0f, 0.0f,
	1.0f, 0.0f,
	0.0f, 1.0f,
	1.0f, 1.0f, 
	};

	unsigned int indices[6] = {
	1, 0, 2,   // First triangle
	2, 3, 1    // Second triangle
	};

	// Creation VAO and activation
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	//Creation and activation VBO. Also configure the VBO with the vertices and his atributs.
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(positions) + sizeof(texCoords), nullptr, GL_STATIC_DRAW);
	
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(positions), positions);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(positions), sizeof(texCoords), texCoords);


	// Configuració dels atributs de vèrtex (posició, coordenades de textura)
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	void* texCoordsOffset = (void*)sizeof(positions);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), texCoordsOffset);
	glEnableVertexAttribArray(1);


	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);


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

update_status ModuleTexture::PreUpdate()
{

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glBindVertexArray(VAO);
	glUseProgram(program);
	return UPDATE_CONTINUE;
}

update_status ModuleTexture::Update()
{

	model = float4x4::identity;
	view = App->camera->LookAt();
	proj = App->camera->GetProjectionMatrix();

	glUniformMatrix4fv(2, 1, GL_TRUE, &model[0][0]);
	glUniformMatrix4fv(3, 1, GL_TRUE, &view[0][0]);
	glUniformMatrix4fv(4, 1, GL_TRUE, &proj[0][0]);

	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

	return UPDATE_CONTINUE;
}

update_status ModuleTexture::PostUpdate()
{
	SDL_GL_SwapWindow(App->GetWindow()->window); //Change the buffer that is showing
	return UPDATE_CONTINUE;
}

bool ModuleTexture::CleanUp()
{
	glDeleteTextures(1, &textureID);
	return true;
}

void ModuleTexture::LoadTexture() {

	HRESULT hr = DirectX::LoadFromDDSFile(imagePath, DirectX::DDS_FLAGS::DDS_FLAGS_NONE, &metadata, image);

	if (FAILED(hr)) {
		hr = DirectX::LoadFromTGAFile(imagePath, DirectX::TGA_FLAGS::TGA_FLAGS_NONE, &metadata, image);
		if (FAILED(hr)) {
			hr = DirectX::LoadFromWICFile(imagePath, DirectX::WIC_FLAGS::WIC_FLAGS_NONE, &metadata, image);
			if (FAILED(hr)) {
				LOG("Failing loading the texture");
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


}

void ModuleTexture::SetUp() {

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


	glFrontFace(GL_CCW);	 // Front faces will be counter clockwise
	glEnable(GL_DEPTH_TEST); // Enable depth test
	glDisable(GL_CULL_FACE); // Disable cull backward faces


	glClearColor(0.7f, 0.7f, 0.7f, 1.0f); //Define the color of background 
	glViewport(0, 0, *(App->GetWindow()->window_width), *(App->GetWindow()->window_height)); //What part of the window will render

}

const float4x4& ModuleTexture::GetModelMatrix() {
	return model;
}

const float4x4& ModuleTexture::GetViewMatrix() {
	return view;
}

const float4x4& ModuleTexture::GetProjectionMatrix() {
	return proj;
}