#pragma once
#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "Shader.h"
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

	Shader ourShader;
	unsigned int VAO;
	unsigned int VBO;
	unsigned int EBO;
	std::vector<unsigned int> VBOarray;

	GLFWwindow* window;
};

