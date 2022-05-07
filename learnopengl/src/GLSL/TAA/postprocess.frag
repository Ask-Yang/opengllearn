#version 330 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D screenTexture;
vec3 ACESFilm(vec3 x)
{
    float a = 2.51f;
    float b = 0.03f;
    float c = 2.43f;
    float d = 0.59f;
    float e = 0.14f;
    return clamp((x*(a*x+b))/(x*(c*x+d)+e), 0, 1);
}


void main()
{
    vec3 col = vec3(0);
    vec2 coord = vec2((TexCoords.x + 1) / 2, (TexCoords.y + 1) / 2);
    col = texture(screenTexture, coord).rgb;
   
    FragColor = vec4(col, 1);
} 