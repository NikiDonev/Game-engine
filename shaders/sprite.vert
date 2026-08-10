#version 330 core
layout (location = 0) in vec2 aPos;
layout (location = 1) in vec4 aColor;
layout (location = 2) in vec2 aTexCoords;
layout (location = 3) in float aTexIndex;

out vec2 TexCoords;
out float TexIndex;
out vec4 Color;

uniform mat4 viewProj;

void main(){
	gl_Position = viewProj * vec4(aPos, 0.0, 1.0);
	TexCoords = aTexCoords;
	TexIndex = aTexIndex;
	Color = aColor;
}

