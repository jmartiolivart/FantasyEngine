#version 460
in vec2 TexCoords;         // Coordenades de textura del vertex shader
out vec4 FragColor;        // Color de sortida del píxel

uniform sampler2D texture_diffuse; // Textura a utilitzar

void main() {
    FragColor = texture(texture_diffuse, TexCoords); // Assignar color segons la textura
}