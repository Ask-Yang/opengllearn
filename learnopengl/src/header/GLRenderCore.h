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
#include "Model.h"
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
	virtual void Run() = 0;
	void enableDepthTest();

protected:
	virtual void initScene() = 0;
	virtual void initResource() = 0;
	void processInput(GLFWwindow* window);

	std::string addShaderProgram(std::string shaderName, std::string vertexShaderPath, std::string fragmentShaderPath);
	std::string addShaderProgram(std::string shaderName, std::string vertexShaderPath, std::string fragmentShaderPath, std::string vertexPrecomplie, std::string fragmentPrecomplie);
	void frameUpdateViewProjectionMatrix();
	

private:
	void initGLFW();
	void initWindow();

protected:
	const unsigned int screenWidth = 800;
	const unsigned int screenHeight = 600;
	std::map<std::string, std::shared_ptr<Shader>> coreShaderArr;

	GLFWwindow* window = nullptr;
	Camera* pCamera = nullptr;
};

