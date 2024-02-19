#version 330 core
out vec4 FragColor;

in vec2 TexCoord;

uniform float colorScale;
uniform sampler2D tex;
uniform sampler2D tex2;

void main()
{
    FragColor = texture(tex, TexCoord);
}