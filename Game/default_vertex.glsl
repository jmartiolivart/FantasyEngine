#version 460

layout(location = 0) in vec3 my_vertex_position;
layout(location = 1) in vec2 my_texture_coords; 

layout(location = 2) uniform mat4 model; 
layout(location = 3) uniform mat4 view;
layout(location = 4) uniform mat4 proj;

out vec2 TexCoords; 

void main()
{
  gl_Position = proj * view * model * vec4(my_vertex_position, 1.0);
  TexCoords = my_texture_coords; 
}