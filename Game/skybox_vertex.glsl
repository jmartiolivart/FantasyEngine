#version 460
layout(location = 0) in vec3 aPos;

out vec3 TexCoords;

uniform mat4 view;
uniform mat4 projection;

void main()
{
    // Remove the translation of skybox
    mat4 rotView = mat4(mat3(view));

    TexCoords = aPos;
    gl_Position = projection * rotView * vec4(aPos, 1.0);
}
