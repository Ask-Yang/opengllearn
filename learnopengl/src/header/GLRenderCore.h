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
	void SetShader(std::string vertexShaderPath, std::string fragmentShaderPath);
private:
	void processInput(GLFWwindow* window);
	void initGLFW();
	void initWindow();
	void buildVAO();

	std::shared_ptr<Shader> coreShader;
	unsigned int VAO = 0;
	unsigned int VBO = 0;
	unsigned int EBO = 0;
	std::vector<unsigned int> VBOarray;

	GLFWwindow* window;
};

