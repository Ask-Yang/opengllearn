#include "Shader.h"

using namespace std;

Shader::Shader(const GLchar* vertexPath, const GLchar* fragmentPath)
{
	readShaderFileToString(vertexPath, fragmentPath);
	compileVertexShader(m_vShaderString);
	compileFragmentShader(m_fShaderString);
	linkShaderProgram();
}

void Shader::readShaderFileToString(const GLchar* vertexPath, const GLchar* fragmentPath)
{
	ifstream vShaderFile;
	ifstream fShaderFile;

	vShaderFile.exceptions(ifstream::failbit | ifstream::badbit);
	fShaderFile.exceptions(ifstream::failbit | ifstream::badbit);

	try
	{
		vShaderFile.open(vertexPath);
		fShaderFile.open(fragmentPath);
		stringstream vShaderStream, fShaderStream;
		vShaderStream << vShaderFile.rdbuf();
		fShaderStream << fShaderFile.rdbuf();
		vShaderFile.close();
		fShaderFile.close();
		m_vShaderString = vShaderStream.str();
		m_fShaderString = fShaderStream.str();
	}
	catch (ifstream::failure e)
	{
		cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
	}
	
}

void Shader::compileVertexShader(string& vShaderString)
{
	const char* vShaderChar = vShaderString.c_str();
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vShaderChar, nullptr);
	glCompileShader(vertexShader);
	checkErrorInformation(vertexShader, GL_COMPILE_STATUS);
}

void Shader::compileFragmentShader(string& fShaderString)
{
	const char* fShaderChar = fShaderString.c_str();
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fShaderChar, nullptr);
	glCompileShader(fragmentShader);
	checkErrorInformation(fragmentShader, GL_COMPILE_STATUS);
}

void Shader::linkShaderProgram()
{
	shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);
	checkErrorInformation(shaderProgram, GL_LINK_STATUS);
	
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
	m_vShaderString.clear();
	m_fShaderString.clear();

	ID = shaderProgram;
}

void Shader::checkErrorInformation(GLuint objectID, GLenum pname)
{
	int success;
	char infoLog[512];
	if (pname == GL_COMPILE_STATUS)
	{
		glGetShaderiv(objectID, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(objectID, 512, nullptr, infoLog);
			cout << "ERROR::SHADER::COMPILATION_FAILED\n" << infoLog << std::endl;
		}
	}
	else if (pname == GL_LINK_STATUS)
	{
		glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
		if (!success) {
			glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
			cout << "ERROR::PROGRAM::LINKED_FAILED\n" << infoLog << std::endl;
		}
	}

}
