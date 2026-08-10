#version 330 core

out vec4 FragColor;
in vec2 TexCoords;

uniform vec4 color;

uniform sampler2D texture1;

void main()
{    
    FragColor = color * texture(texture1, TexCoords);
}