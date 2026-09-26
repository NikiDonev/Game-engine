#version 330 core
out vec4 FragColor;

in vec4 Color;

void main() {
	
	FragColor = Color * vec4(1.0, 0.0, 0.0, 1.0);
}