#pragma once
#include "glad/glad.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

class Shader {
public:
	unsigned int ID;
	Shader(std::string vertexPath, std::string fragmentPath);

	void use() {
		glUseProgram(ID);
	}
	void setBool(const std::string& shaderVariableName, bool value) const {
		glUniform1i(glGetUniformLocation(ID, shaderVariableName.c_str()), (int)value);
	}
	void setTexture(const std::string& shaderVariableName, int textureSlotNumber) const {
		glUniform1i(glGetUniformLocation(ID, shaderVariableName.c_str()), textureSlotNumber);
	}
	void setInt(const std::string& shaderVariableName, int value) const {
		glUniform1i(glGetUniformLocation(ID, shaderVariableName.c_str()), value);
	}
	void setFloat(const std::string& shaderVariableName, float value) const {
		glUniform1f(glGetUniformLocation(ID, shaderVariableName.c_str()), value);
	}
	void setMat4(std::string shaderVariableName, glm::mat4 matrix) {
		glUniformMatrix4fv(glGetUniformLocation(ID, shaderVariableName.c_str()), 1, GL_FALSE, glm::value_ptr(matrix));
	}
private:
	void readShaderFileToString(std::string vertexPath, std::string fragmentPath);

	void compileVertexShader(std::string &vShaderString);
	void compileFragmentShader(std::string &fShaderString);
	void linkShaderProgram();
	void deleteShaderSource();
	void checkErrorInformation(GLuint objectID, GLenum pname);

	unsigned int shaderProgram = 0;
	unsigned int fragmentShader = 0;
	unsigned int vertexShader = 0;
	std::string m_vShaderString;
	std::string m_fShaderString;
};