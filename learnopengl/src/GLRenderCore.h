#pragma once
#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include <vector>
#include <iostream>
void framebuffer_size_callback(GLFWwindow* window, int width, int height);


class GLRenderCore
{
public:
	GLRenderCore();
	~GLRenderCore();
	void Init();
	void Run();
private:
	void processInput(GLFWwindow* window);
	void initGLFW();
	void initWindow();
	void initShader();
	void buildVAO();
	void compileVertexShader();
	void compileFragmentShader();
	void linkShaderProgram();
	void activeShaderProgram();
	void checkErrorInformation(GLuint objectID, GLenum pname);

	unsigned int VAO;
	unsigned int VBO;
	std::vector<unsigned int> VBOarray;
	unsigned int shaderProgram = 0;
	unsigned int fragmentShader = 0;
	unsigned int vertexShader = 0;
	GLFWwindow* window;
};

