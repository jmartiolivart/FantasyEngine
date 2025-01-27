#include "ModuleSkybox.h"
#include "Application.h"
#include "ModuleCamera.h"
#include "Shader.h"
#include <GL/glew.h>
#include "./DirectXTex-oct2024/DirectXTex/DirectXTex.h"
#include "./math-library/Math/float4x4.h"
#include <string>

// Si vas a fer servir la versió lliure de StringToWString, avisa-ho:
std::wstring StringToWString(const std::string& str) {
    return std::wstring(str.begin(), str.end());
}

ModuleSkybox::ModuleSkybox()
{
}

ModuleSkybox::~ModuleSkybox()
{
}

// ----------------------------------------------------------------------------------
// INIT
// ----------------------------------------------------------------------------------
bool ModuleSkybox::Init()
{
    // 1. Geometria d'un cub "invertit" (perquè la cara interior sigui visible).
    float skyboxVertices[] = {
        // Right (+X)
         1.0f, -1.0f, -1.0f,
         1.0f,  1.0f, -1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f, -1.0f,  1.0f,
         // Left (-X)
         -1.0f, -1.0f, -1.0f,
         -1.0f, -1.0f,  1.0f,
         -1.0f,  1.0f,  1.0f,
         -1.0f, -1.0f, -1.0f,
         -1.0f,  1.0f,  1.0f,
         -1.0f,  1.0f, -1.0f,
         // Top (+Y)
         -1.0f,  1.0f, -1.0f,
         -1.0f,  1.0f,  1.0f,
          1.0f,  1.0f,  1.0f,
         -1.0f,  1.0f, -1.0f,
          1.0f,  1.0f,  1.0f,
          1.0f,  1.0f, -1.0f,
          // Bottom (-Y)
          -1.0f, -1.0f, -1.0f,
           1.0f, -1.0f, -1.0f,
           1.0f, -1.0f,  1.0f,
          -1.0f, -1.0f, -1.0f,
           1.0f, -1.0f,  1.0f,
          -1.0f, -1.0f,  1.0f,
          // Front (+Z)
          -1.0f, -1.0f,  1.0f,
           1.0f, -1.0f,  1.0f,
           1.0f,  1.0f,  1.0f,
          -1.0f, -1.0f,  1.0f,
           1.0f,  1.0f,  1.0f,
          -1.0f,  1.0f,  1.0f,
          // Back (-Z)
          -1.0f, -1.0f, -1.0f,
          -1.0f,  1.0f, -1.0f,
           1.0f,  1.0f, -1.0f,
          -1.0f, -1.0f, -1.0f,
           1.0f,  1.0f, -1.0f,
           1.0f, -1.0f, -1.0f
    };

    // VAO/VBO per al cub
    glGenVertexArrays(1, &skyboxVAO);
    glGenBuffers(1, &skyboxVBO);
    glBindVertexArray(skyboxVAO);

    glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), skyboxVertices, GL_STATIC_DRAW);

    // Atribut posició (location = 0 al shader)
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(
        0,                  // location = 0
        3,                  // x,y,z
        GL_FLOAT,
        GL_FALSE,
        3 * sizeof(float),
        (void*)0
    );

    glBindVertexArray(0);

    // 2. Carregar les imatges per a la cubemap
    std::string facesCubemap[6] = {
        "Assets/px.png", // Right
        "Assets/nx.png", // Left
        "Assets/py.png", // Top
        "Assets/ny.png", // Bottom
        "Assets/pz.png", // Front
        "Assets/nz.png"  // Back
    };

    glGenTextures(1, &cubemapTexture);
    glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);

    // Paràmetres habituals d'una cubemap
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    // (Opcional) per suavitzar costures:
    // glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);

    HRESULT hr;
    DirectX::ScratchImage image;
    DirectX::TexMetadata metadata;

    for (unsigned int i = 0; i < 6; i++)
    {
        // Convertim la ruta de std::string a std::wstring
        std::wstring wpath = StringToWString(facesCubemap[i]);

        // Anem provant LoadFromDDS, LoadFromTGA i finalment LoadFromWIC
        hr = DirectX::LoadFromDDSFile(wpath.c_str(), DirectX::DDS_FLAGS_NONE, &metadata, image);
        if (FAILED(hr)) {
            hr = DirectX::LoadFromTGAFile(wpath.c_str(), DirectX::TGA_FLAGS_NONE, &metadata, image);
        }
        if (FAILED(hr)) {
            hr = DirectX::LoadFromWICFile(wpath.c_str(), DirectX::WIC_FLAGS_NONE, &metadata, image);
        }
        if (FAILED(hr)) {
            LOG("Failed to load cubemap face: %s", facesCubemap[i].c_str());
            return false;
        }
        else {
            LOG("Successfully loaded cubemap face: %s", facesCubemap[i].c_str());
        }

        // Recuperem la imatge
        const DirectX::Image* img = image.GetImage(0, 0, 0);
        if (!img) {
            LOG("DirectX::GetImage returned null in face: %s", facesCubemap[i].c_str());
            return false;
        }

        // Triar formats interns
        GLint internalFormat = GL_RGBA8;
        GLenum format = GL_RGBA;
        GLenum type = GL_UNSIGNED_BYTE;

        switch (metadata.format)
        {
        case DXGI_FORMAT_R8G8B8A8_UNORM_SRGB:
        case DXGI_FORMAT_R8G8B8A8_UNORM:
            internalFormat = GL_RGBA8;
            format = GL_RGBA;
            type = GL_UNSIGNED_BYTE;
            break;
        case DXGI_FORMAT_B8G8R8A8_UNORM_SRGB:
        case DXGI_FORMAT_B8G8R8A8_UNORM:
            internalFormat = GL_RGBA8;
            format = GL_BGRA;
            type = GL_UNSIGNED_BYTE;
            break;
        case DXGI_FORMAT_B5G6R5_UNORM:
            internalFormat = GL_RGB8;
            format = GL_BGR;
            type = GL_UNSIGNED_BYTE;
            break;
        default:
            LOG("Unsupported format in cubemap face: %s", facesCubemap[i].c_str());
            return false;
        }

        glTexImage2D(
            GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
            0,                          // level
            internalFormat,            // GL_RGBA8 ...
            static_cast<GLsizei>(metadata.width),
            static_cast<GLsizei>(metadata.height),
            0,                          // border
            format,                     // GL_RGBA / GL_BGRA
            type,                       // GL_UNSIGNED_BYTE
            img->pixels
        );
    }

    // 3. Crear el programa (shader) de skybox
    std::string vertexShaderSkybox = Shader::readShader("skybox_vertex.glsl");
    std::string fragmentShaderSkybox = Shader::readShader("skybox_fragment.glsl");

    skyboxProgram = Shader::CreateShader(vertexShaderSkybox, fragmentShaderSkybox);
    if (skyboxProgram == 0)
    {
        LOG("Failed creating skybox shader program");
        return false;
    }

    // Si tot ha anat bé, retornem true
    return true;
}

// ----------------------------------------------------------------------------------
// PreUpdate
// ----------------------------------------------------------------------------------
update_status ModuleSkybox::PreUpdate()
{
    return UPDATE_CONTINUE;
}

// ----------------------------------------------------------------------------------
// UPDATE
// ----------------------------------------------------------------------------------
update_status ModuleSkybox::Update()
{
    // Configurar el test de profunditat per permetre fragments amb profunditat igual
    glDepthFunc(GL_LEQUAL);
    glDepthMask(GL_FALSE); // Desactiva l'escriptura al depth buffer

    // Activar el face culling i cullar la cara frontal (per veure la interior del cub)
    glEnable(GL_CULL_FACE);
    glCullFace(GL_FRONT);

    // Usar el shader del skybox
    glUseProgram(skyboxProgram);

    // Obtenir les localitzacions dels uniforms (si no les tens precalculades)
    GLint locView = glGetUniformLocation(skyboxProgram, "view");
    GLint locProjection = glGetUniformLocation(skyboxProgram, "projection");

    // Creem la matriu de vista (no toquem la translació perquè ja s'elimina al shader)
    math::float4x4 skyboxView = App->camera->LookAt();

    // Assignem les matrius de vista i projecció als uniforms del shader
    if (locView != -1)
        glUniformMatrix4fv(locView, 1, GL_TRUE, &skyboxView[0][0]);

    if (locProjection != -1)
        glUniformMatrix4fv(locProjection, 1, GL_TRUE, &App->camera->GetProjectionMatrix()[0][0]);

    // Activar la cubemap al Texture Unit 0
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);

    // Dibuixar el cub del skybox
    glBindVertexArray(skyboxVAO);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glBindVertexArray(0);

    // Restaurar els estats d'OpenGL per la resta de l'escena
    glCullFace(GL_BACK);
    glDisable(GL_CULL_FACE);
    glDepthMask(GL_TRUE);
    glDepthFunc(GL_LESS);


    return UPDATE_CONTINUE;
}


// ----------------------------------------------------------------------------------
// PostUpdate
// ----------------------------------------------------------------------------------
update_status ModuleSkybox::PostUpdate()
{
    return UPDATE_CONTINUE;
}

// ----------------------------------------------------------------------------------
// CleanUp
// ----------------------------------------------------------------------------------
bool ModuleSkybox::CleanUp()
{
    LOG("Cleaning up Skybox module...");

    // Eliminar buffers, si cal
    if (skyboxVBO) {
        glDeleteBuffers(1, &skyboxVBO);
        skyboxVBO = 0;
    }
    if (skyboxVAO) {
        glDeleteVertexArrays(1, &skyboxVAO);
        skyboxVAO = 0;
    }
    if (cubemapTexture) {
        glDeleteTextures(1, &cubemapTexture);
        cubemapTexture = 0;
    }
    if (skyboxProgram) {
        glDeleteProgram(skyboxProgram);
        skyboxProgram = 0;
    }
    return true;
}
