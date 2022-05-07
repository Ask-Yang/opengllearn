#include "Shader.h"

using namespace std;

Shader::Shader(string vertexPath, string fragmentPath)
{
	readShaderFileToString(vertexPath, fragmentPath);
	compileVertexShader(m_vShaderString);
	compileFragmentShader(m_fShaderString);
	linkShaderProgram();
	deleteShaderSource();
}

Shader::Shader(std::string vertexPath, std::string fragmentPath, std::string vertexPrecomplie, std::string fragmentPrecomplie)
{
	readShaderFileToString(vertexPath, fragmentPath);
	insertPreComplieIntoShaderStr(vertexPrecomplie, fragmentPrecomplie);
	compileVertexShader(m_vShaderString);
	compileFragmentShader(m_fShaderString);
	linkShaderProgram();
	deleteShaderSource();
}

void Shader::use() 
{
	glUseProgram(ID);
	for (int i=0;i<textureSlotMap.size();i++)
	{
		glActiveTexture(GL_TEXTURE0 + i);
		glBindTexture(GL_TEXTURE_2D, textureSlotMap[i]);
	}
}
void Shader::setBool(const std::string shaderVariableName, bool value) 
{
	use();
	glUniform1i(glGetUniformLocation(ID, shaderVariableName.c_str()), (int)value);
}
void Shader::setTexture(const std::string shaderVariableName, int textureSlotNumber) 
{
	use();
	glUniform1i(glGetUniformLocation(ID, shaderVariableName.c_str()), textureSlotNumber);
}
void Shader::bindAndSetTexture(const std::string shaderVariableName, int textureID)
{
	if (shaderTextureSlot < 10)
	{
		use();
		textureSlotMap[shaderTextureSlot] = textureID;
		glUniform1i(glGetUniformLocation(ID, shaderVariableName.c_str()), shaderTextureSlot++);
	}
	else
		cout << "Texture slot is not enough" << endl;
}
void Shader::setInt(const std::string shaderVariableName, int value) 
{
	use();
	glUniform1i(glGetUniformLocation(ID, shaderVariableName.c_str()), value);
}
void Shader::setFloat(const std::string shaderVariableName, float value) 
{
	use();
	glUniform1f(glGetUniformLocation(ID, shaderVariableName.c_str()), value);
}
void Shader::setMat4(const std::string shaderVariableName, glm::mat4 matrix)
{
	use();
	glUniformMatrix4fv(glGetUniformLocation(ID, shaderVariableName.c_str()), 1, GL_FALSE, glm::value_ptr(matrix));
}
void Shader::setMat3(const std::string shaderVariableName, glm::mat3 matrix)
{
	use();
	glUniformMatrix3fv(glGetUniformLocation(ID, shaderVariableName.c_str()), 1, GL_FALSE, glm::value_ptr(matrix));
}
void Shader::setVec3(const std::string shaderVariableName, float value1, float value2, float valu3) 
{
	use();
	glUniform3f(glGetUniformLocation(ID, shaderVariableName.c_str()), value1, value2, valu3);
}

void Shader::setVec3(const std::string shaderVariableName, glm::vec3 vec)
{
	use();
	glUniform3f(glGetUniformLocation(ID, shaderVariableName.c_str()), vec.x, vec.y, vec.z);
}

void Shader::readShaderFileToString(string vertexPath, string fragmentPath)
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

void Shader::insertPreComplieIntoShaderStr(std::string vertexPrecomplie, std::string fragmentPrecomplie)
{
	for (int i = 0; i < m_vShaderString.size(); i++)
		if (m_vShaderString[i] == '\n')
		{
			m_vShaderString.insert(i + 1, vertexPrecomplie);
			break;
		}
	for (int i = 0; i < m_fShaderString.size(); i++)
		if (m_fShaderString[i] == '\n')
		{
			m_fShaderString.insert(i + 1, fragmentPrecomplie);
			break;
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

	ID = shaderProgram;
}

void Shader::deleteShaderSource()
{
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
	m_vShaderString.clear();
	m_fShaderString.clear();
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
