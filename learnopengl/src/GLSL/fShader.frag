#version 330 core
out vec4 FragColor;

in vec3 Normal;
in vec3 WorldPos;
in vec2 TexCoords;

uniform vec3 lightColor;
uniform vec3 viewPos;

struct Material{
	sampler2D objectDiffuse;
	sampler2D objectSpecular;
	float shininess;
};
uniform Material material;

struct Light{
	vec3 position;
	vec3 ambientStrength;
	vec3 diffuseStrength;
	vec3 specularStrength;
};
uniform Light light;

void main()
{
	vec3 worldNormal = normalize(Normal);
	vec3 lightDir = normalize(light.position - WorldPos);

	vec3 ambientLight = light.ambientStrength * lightColor;

	float diff = max(dot(worldNormal, lightDir), 0.0);
	vec3 diffuseLight = light.diffuseStrength * diff * lightColor;

	vec3 viewDir = normalize(viewPos - WorldPos);
	vec3 reflectDir = reflect(-lightDir, worldNormal);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
	vec3 specularLight = light.specularStrength * spec * lightColor;

	vec3 objectDiffuse = vec3(texture(material.objectDiffuse, TexCoords));
	vec3 result = ambientLight * objectDiffuse
	+ diffuseLight * objectDiffuse
	+ specularLight * vec3(texture(material.objectSpecular, TexCoords));
	FragColor = vec4(result, 1.0);
	//FragColor = vec4(1,1,1,1);
}