#include "lightStruct.h"

DirLight::DirLight(std::string nameIn)
	:Light(nameIn)
{
}

void DirLight::bindVariableToShader(Shader& shader)
{
	shader.setVec3(direction.first, direction.second);
	shader.setVec3(ambient.first, ambient.second);
	shader.setVec3(diffuse.first, diffuse.second);
	shader.setVec3(specular.first, specular.second);
}

PointLight::PointLight(std::string nameIn)
	:Light(nameIn)
{
}


void PointLight::bindVariableToShader(Shader& shader)
{
	shader.setVec3(position.first, position.second);
	shader.setVec3(ambient.first, ambient.second);
	shader.setVec3(diffuse.first, diffuse.second);
	shader.setVec3(specular.first, specular.second);
	shader.setFloat(constant.first, constant.second);
	shader.setFloat(linear.first, linear.second);
	shader.setFloat(quadratic.first, quadratic.second);
}

SpotLight::SpotLight(std::string nameIn)
	:Light(nameIn)
{
}



void SpotLight::bindVariableToShader(Shader& shader)
{
	shader.setVec3(position.first, position.second);
	shader.setVec3(direction.first, direction.second);
	shader.setVec3(ambient.first, ambient.second);
	shader.setVec3(diffuse.first, diffuse.second);
	shader.setVec3(specular.first, specular.second);
	shader.setFloat(constant.first, constant.second);
	shader.setFloat(linear.first, linear.second);
	shader.setFloat(quadratic.first, quadratic.second);
	shader.setFloat(cutOff.first, cutOff.second);
	shader.setFloat(outerCutOff.first, outerCutOff.second);
}

Light::Light(std::string nameIn)
{
	name = nameIn;
}

Light::Light()
{
}

