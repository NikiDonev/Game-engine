#version 330 core
out vec4 FragColor;

in vec2 TexCoords;
in float TexIndex;
in vec4 Color;

uniform sampler2D textures[32];


void main() {
	float radius = 0.2;
	float radius2 = 0.3;
	vec2 center = vec2(0.5, 0.5);
	float dist = distance(center, TexCoords);

	dist = 0.1/dist;
	FragColor = vec4(1.0, 0.0, 0.0, dist);
	
}