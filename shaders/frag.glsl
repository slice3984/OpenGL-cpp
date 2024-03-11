#version 330 core
out vec4 FragColor;

in vec2 TexCoord;
in vec3 Normal;

uniform vec4 factorsBaseColor;
uniform float colorScale;
uniform sampler2D baseColor;

void main()
{
    vec3 lightDir = normalize(vec3(0.0, 1.0, 1.0)); // Example light direction
    float diffuse = max(dot(Normal, lightDir), 0.0); // Lambertian diffuse lighting

    // Ambient lighting
    vec3 ambientColor = vec3(1, 1, 1); // Ambient color
    vec3 textureColor = factorsBaseColor.rgb * texture(baseColor, TexCoord).rgb;
    vec3 ambientFinalColor = textureColor * ambientColor;

    // Diffuse lighting
    vec3 diffuseFinalColor = textureColor * diffuse;

    // Combine ambient and diffuse lighting
    vec3 finalColor = ambientFinalColor + diffuseFinalColor;

    FragColor = vec4(finalColor, 1.0);
}