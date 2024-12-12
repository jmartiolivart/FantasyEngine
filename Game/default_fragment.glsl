#version 460
in vec2 TexCoords;
layout(binding = 5) uniform sampler2D my_texture;
out vec4 color;
void main()
{
 color = texture(my_texture, TexCoords);
}
