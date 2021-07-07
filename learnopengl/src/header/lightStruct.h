#pragma once
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#include "Shader.h"

#include <string>
#include <map>
struct Light {
	std::string name;
	Light(std::string nameIn);
	Light();
};

struct DirLight : public Light {
	std::pair<std::string, glm::vec3> direction = std::make_pair<std::string, glm::vec3>(name + ".direction", glm::vec3(-0.2f, -1.0f, -0.3f));
	std::pair<std::string, glm::vec3> ambient = std::make_pair<std::string, glm::vec3>(name + ".ambient", glm::vec3(0.05f, 0.05f, 0.05f));
	std::pair<std::string, glm::vec3> diffuse = std::make_pair<std::string, glm::vec3>(name + ".diffuse", glm::vec3(0.4f, 0.4f, 0.4f));
	std::pair<std::string, glm::vec3> specular = std::make_pair<std::string, glm::vec3>(name + ".specular", glm::vec3(0.5f, 0.5f, 0.5f));
	DirLight(std::string nameIn);
	void bindVariableToShader(Shader& shader);
};

struct PointLight : public Light {
	std::pair<std::string, glm::vec3> position = std::make_pair<std::string, glm::vec3>(name + ".position", glm::vec3(1, 1, 1));

	std::pair<std::string, float> constant = std::make_pair<std::string, float>(name + ".constant", 1.0f);
	std::pair<std::string, float> linear = std::make_pair<std::string, float>(name + ".linear", 0.09f);
	std::pair<std::string, float> quadratic = std::make_pair<std::string, float>(name + ".quadratic", 0.032f);

	std::pair<std::string, glm::vec3> ambient = std::make_pair<std::string, glm::vec3>(name + ".ambient", glm::vec3(0.05f, 0.05f, 0.05f));
	std::pair<std::string, glm::vec3> diffuse = std::make_pair<std::string, glm::vec3>(name + ".diffuse", glm::vec3(0.8f, 0.8f, 0.8f));
	std::pair<std::string, glm::vec3> specular = std::make_pair<std::string, glm::vec3>(name + ".specular", glm::vec3(1.0f, 1.0f, 1.0f));
	PointLight(std::string nameIn);
	void bindVariableToShader(Shader& shader);
};

struct SpotLight : public Light {
	std::pair<std::string, glm::vec3> position = std::make_pair<std::string, glm::vec3>(name + ".position", glm::vec3(1, 1, 1));
	std::pair<std::string, glm::vec3> direction = std::make_pair<std::string, glm::vec3>(name + ".direction", glm::vec3(1, 1, 1));

	std::pair<std::string, float> constant = std::make_pair<std::string, float>(name + ".constant", 1.0f);
	std::pair<std::string, float> linear = std::make_pair<std::string, float>(name + ".linear", 0.09f);
	std::pair<std::string, float> quadratic = std::make_pair<std::string, float>(name + ".quadratic", 0.032f);
	std::pair<std::string, float> cutOff = std::make_pair<std::string, float>(name + ".cutOff", glm::cos(glm::radians(12.5f)));
	std::pair<std::string, float> outerCutOff = std::make_pair<std::string, float>(name + ".outerCutOff", glm::cos(glm::radians(15.0f)));

	std::pair<std::string, glm::vec3> ambient = std::make_pair<std::string, glm::vec3>(name + ".ambient", glm::vec3(0.0f, 0.0f, 0.0f));
	std::pair<std::string, glm::vec3> diffuse = std::make_pair<std::string, glm::vec3>(name + ".diffuse", glm::vec3(1.0f, 1.0f, 1.0f));
	std::pair<std::string, glm::vec3> specular = std::make_pair<std::string, glm::vec3>(name + ".specular", glm::vec3(1.0f, 1.0f, 1.0f));
	SpotLight(std::string nameIn);
	void bindVariableToShader(Shader& shader);
};