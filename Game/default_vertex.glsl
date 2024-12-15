#version 460
layout(location = 0) in vec3 vertex_position; // Posició dels vèrtexs
layout(location = 1) in vec2 texture_coords;  // Coordenades de textura

layout(location = 2) uniform mat4 model; // Matriu model
layout(location = 3) uniform mat4 view;  // Matriu vista
layout(location = 4) uniform mat4 proj;  // Matriu projecció

out vec2 TexCoords; // Coordenades de textura cap al fragment shader

void main() {
    gl_Position = proj * view * model * vec4(vertex_position, 1.0); // Transformar
    TexCoords = texture_coords; // Passar coordenades de textura
}
