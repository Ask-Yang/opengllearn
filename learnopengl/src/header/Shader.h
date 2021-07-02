#pragma once
#include "glad/glad.h"
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
	void setTexture(const std::string& shaderVariableName, int value) const {
		glUniform1i(glGetUniformLocation(ID, shaderVariableName.c_str()), value);
	}
	void setInt(const std::string& shaderVariableName, int value) const {
		glUniform1i(glGetUniformLocation(ID, shaderVariableName.c_str()), value);
	}
	void setFloat(const std::string& shaderVariableName, float value) const {
		glUniform1f(glGetUniformLocation(ID, shaderVariableName.c_str()), value);
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