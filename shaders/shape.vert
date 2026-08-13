#version 330 core
layout (location = 0) in vec2 aPos;
layout (location = 1) in vec4 aColor;

out vec4 Color;

uniform mat4 viewProj;

void main(){
	gl_Position = viewProj * vec4(aPos, 0.0, 1.0);
	Color = aColor;
}