#pragma once
#include "glad/glad.h"
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

class Shader {
public:
	unsigned int ID;
	Shader(const GLchar* vertexPath, const GLchar* fragmentPath);

	void use() {
		glUseProgram(ID);
	}
	void setBool(const std::string& name, bool value) const {
		glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
	}
	void setInt(const std::string& name, int value) const {
		glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
	}
	void setFloat(const std::string& name, float value) const {
		glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
	}

private:
	void readShaderFileToString(const GLchar* vertexPath, const GLchar* fragmentPath);

	void compileVertexShader(std::string &vShaderString);
	void compileFragmentShader(std::string &fShaderString);
	void linkShaderProgram();
	void checkErrorInformation(GLuint objectID, GLenum pname);

	unsigned int shaderProgram = 0;
	unsigned int fragmentShader = 0;
	unsigned int vertexShader = 0;
	std::string m_vShaderString;
	std::string m_fShaderString;
};