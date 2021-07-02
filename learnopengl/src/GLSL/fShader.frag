#version 330 core
in vec3 ourColor;
in vec2 TexCoord;

out vec4 FragColor;

uniform sampler2D texture1;
uniform float uniColor;

void main()
{
	FragColor = texture(texture1, TexCoord);
	// FragColor =  mix(texture(texture1, TexCoord), texture(texture2, TexCoord), 0.2);
	//FragColor = vec4(uniColor, 0, 0, 1);
}