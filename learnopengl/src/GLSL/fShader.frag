#version 330 core
out vec4 FragColor;

in vec3 Normal;
in vec3 WorldPos;

uniform vec3 objectColor;
uniform vec3 lightPos;
uniform vec3 lightColor;
uniform vec3 viewPos;

void main()
{
	float ambientStrength = 0.1;
	vec3 ambientLight = ambientStrength * lightColor;

	vec3 worldNormal = normalize(Normal);
	vec3 lightDir = normalize(lightPos - WorldPos);
	float diff = max(dot(worldNormal, lightDir), 0.0);
	vec3 diffuseLight = diff * lightColor;

	float specularStrength = 0.5;
	vec3 viewDir = normalize(viewPos - WorldPos);
	vec3 reflectDir = reflect(-lightDir, worldNormal);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), 128);
	vec3 specularLight = specularStrength * spec * lightColor;

	vec3 result = (ambientLight + diffuseLight + specularLight) * objectColor;
	FragColor = vec4(result, 1.0);
}