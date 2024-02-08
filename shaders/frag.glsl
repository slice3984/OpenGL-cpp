#version 330 core
out vec4 FragColor;
in vec4 color;

uniform float colorScale;

void main()
{
    FragColor = colorScale * color;
}