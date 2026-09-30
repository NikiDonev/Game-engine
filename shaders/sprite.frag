#version 330 core
out vec4 FragColor;

in vec2 TexCoords;
in float TexIndex;
in vec4 Color;

uniform sampler2D textures[32];

void main() {
	int index = int(TexIndex);
	FragColor = Color * texture(textures[index], TexCoords);
}