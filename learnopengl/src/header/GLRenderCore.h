#pragma once
#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#include "Shader.h"
#include "Texture.h"
#include "Camera.h"
#include "RenderPass.h"
#include "RPCube.h"
#include "RPLight.h"
#include <vector>
#include <map>
#include <iostream>
void framebuffer_size_callback(GLFWwindow* window, int width, int height);


class GLRenderCore
{
public:
	GLRenderCore();
	~GLRenderCore();
	void Init();
	void Run();
	void enableDepthTest();
	void enableTexture(std::string textureName);

	std::shared_ptr<Shader> getShaderProgram(std::string shaderName) {
		return coreShaderArr[shaderName];
	}
	std::shared_ptr<Texture2D> getTexture(std::string textureName) {
		return coreTexture2dArr[textureName];
	}
	unsigned int getVBO(std::string VBOName) {
		return coreVBOArr[VBOName];
	}

private:
	void processInput(GLFWwindow* window);
	void initGLFW();
	void initWindow();
	void initScence();
	void initResource();
	void initVBO();
	void initTexture();
	void initShader();

	std::string addShaderProgram(std::string shaderName, std::string vertexShaderPath, std::string fragmentShaderPath);
	std::string addTexture(std::string textureName, std::string texturePath, unsigned int GL_COLOR_FORMAT_MACRO);
	std::string addVBO(std::string VBOName, float* vertices, unsigned int verticesSize);
	void updateMVPMatrix();
	void currentFrameObjectUpdate();

private:
	const unsigned int screenWidth = 800;
	const unsigned int screenHeight = 600;
	std::map<std::string, std::shared_ptr<Shader>> coreShaderArr;
	std::map<std::string, unsigned int> coreVBOArr;
	std::map<std::string, std::shared_ptr<Texture2D>> coreTexture2dArr;
	std::map<std::string, std::shared_ptr<RenderPass>> coreRenderPassArr;

	GLFWwindow* window = nullptr;
};

