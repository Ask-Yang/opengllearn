#version 330 core
out vec4 FragColor;

in vec3 Normal;
in vec3 FragPos;
in vec2 TexCoords;

uniform vec3 lightColor;
uniform vec3 viewPos;

struct Material{
	sampler2D diffuse;
	sampler2D specular;
	float shininess;
};
uniform Material material;

struct DirLight{
	vec3 direction;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};
uniform DirLight dirLight;

struct PointLight{
	vec3 position;
	float constant;
	float linear;
	float quadratic;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};
#define NR_POINT_LIGHTS 4
uniform PointLight pointLights[NR_POINT_LIGHTS];

struct SpotLight{
	vec3 position;
	vec3 direction;
	float constant;
	float linear;
	float quadratic;
	float cutOff;
	float outerCutOff;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};
uniform SpotLight spotLight;

vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir, vec3 objectDiffuse, vec3 objectSpecular);
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir, vec3 objectDiffuse, vec3 objectSpecular);
vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir, vec3 objectDiffuse, vec3 objectSpecular);

void main()
{
	vec3 objectDiffuse = vec3(texture(material.diffuse, TexCoords));
	vec3 norm = normalize(Normal);
	vec3 viewDir = normalize(viewPos - FragPos);
	vec3 objectSpecular = vec3(texture(material.specular, TexCoords));

	vec3 DirLightColor = CalcDirLight(dirLight, norm, viewDir, objectDiffuse, objectSpecular);
	vec3 PointLightColor = vec3(0,0,0);
	for(int i=0;i<NR_POINT_LIGHTS;i++)
		PointLightColor += CalcPointLight(pointLights[i], norm, FragPos, viewDir, objectDiffuse, objectSpecular);
	vec3 SpotLightColor = CalcSpotLight(spotLight, norm, FragPos, viewDir, objectDiffuse, objectSpecular);
	vec3 result = DirLightColor + PointLightColor + SpotLightColor;
	FragColor = vec4(result, 1);
}

vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir, vec3 objectDiffuse, vec3 objectSpecular)
{
	vec3 lightDir = normalize(-light.direction);
	float diff = max(dot(lightDir, normal), 0);
	vec3 reflectDir = reflect(-lightDir, normal);
	float spec = pow(max(dot(reflectDir, viewDir), 0), material.shininess);
	vec3 ambient = light.ambient * vec3(objectDiffuse);
	vec3 diffuse = light.diffuse * diff * vec3(objectDiffuse);
	vec3 specular = light.specular * spec * vec3(objectSpecular);
	return  (ambient + diffuse + specular);
}

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir, vec3 objectDiffuse, vec3 objectSpecular)
{
	vec3 lightDir = normalize(light.position - fragPos);
	float diff = max(dot(normal, lightDir), 0);

	vec3 reflectDir = reflect(-lightDir, normal);
	float spec = pow(max(dot(viewDir, reflectDir), 0), material.shininess);
	
	float lightDistance = length(light.position - fragPos);
	float attenuation = 1.0 / (light.constant + light.linear * lightDistance + light.quadratic * (lightDistance * lightDistance));
	
	vec3 ambient = light.ambient * objectDiffuse * attenuation;
	vec3 diffuse = light.diffuse * diff * objectDiffuse * attenuation;
	vec3 specular = light.specular * spec * objectSpecular * attenuation;
	return (ambient + diffuse + specular);
}

vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir, vec3 objectDiffuse, vec3 objectSpecular)
{
	vec3 lightDir = normalize(light.position - fragPos);
	float diff = max(dot(lightDir, normal), 0);
	
	vec3 reflectDir = reflect(-lightDir, normal);
	float spec = pow(max(dot(reflectDir, viewDir),0) , material.shininess);

	float costheta = dot(-lightDir, light.direction);
	float epsilion = light.cutOff - light.outerCutOff;
	float intensity = clamp((costheta - light.outerCutOff)/epsilion, 0, 1);
	intensity = intensity / (light.constant + light.linear * intensity + light.quadratic * intensity * intensity);

	vec3 ambient = light.ambient * objectDiffuse * intensity;
	vec3 diffuse = light.diffuse * objectDiffuse * intensity;
	vec3 specular = light.specular * objectSpecular * intensity;
	return (ambient + diffuse + specular);
}