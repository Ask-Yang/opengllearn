#pragma once
#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#include "Shader.h"
#include "Texture.h"
#include "Camera.h"
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
	std::shared_ptr<Texture2D> AddTexture(std::string texturePath, unsigned int GL_COLOR_FORMAT_MACRO);
	void enableDepthTest() {
		glEnable(GL_DEPTH_TEST);
	}

private:
	void processInput(GLFWwindow* window);
	void initGLFW();
	void initWindow();
	void buildVAO();

	const unsigned int screenWidth = 800;
	const unsigned int screenHeight = 600;
	std::vector<std::shared_ptr<Texture2D>> coreTexture2dArr;
	std::shared_ptr<Shader> coreShader;
	unsigned int VAO = 0;
	unsigned int VBO = 0;
	unsigned int EBO = 0;
	std::vector<unsigned int> VBOarray;

	GLFWwindow* window = nullptr;
};

