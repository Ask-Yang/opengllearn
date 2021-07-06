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
#include <iostream>
void framebuffer_size_callback(GLFWwindow* window, int width, int height);


class GLRenderCore
{
public:
	GLRenderCore();
	~GLRenderCore();
	void Init();
	void Run();

	std::shared_ptr<Texture2D> AddTexture(std::string texturePath, unsigned int GL_COLOR_FORMAT_MACRO);
	void enableDepthTest();

private:
	void processInput(GLFWwindow* window);
	void initGLFW();
	void initWindow();
	void initScence();
	void updateMVPMatrix();
	void currentFrameObjectUpdate();

private:
	const unsigned int screenWidth = 800;
	const unsigned int screenHeight = 600;
	std::vector<std::shared_ptr<Texture2D>> coreTexture2dArr;
	std::vector<std::shared_ptr<RenderPass>> coreRenderPassArr;

	GLFWwindow* window = nullptr;
};

