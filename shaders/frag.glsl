#version 330 core
out vec4 FragColor;

in vec4 color;
in vec2 TexCoord;

uniform float colorScale;
uniform sampler2D tex;
uniform sampler2D tex2;

void main()
{
    FragColor = mix(texture(tex, TexCoord), texture(tex2, TexCoord), 0.2) * color * colorScale;
}