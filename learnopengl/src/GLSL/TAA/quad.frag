#version 330 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D screenTexture;


void main()
{
    vec3 col = vec3(0);
    vec2 coord = vec2((TexCoords.x + 1) / 2, (TexCoords.y + 1) / 2);
    col = texture(screenTexture, coord).rgb;
    FragColor = vec4(col, 1);
} 