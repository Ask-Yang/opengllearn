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

class RenderPass
{
public:
	virtual void init() = 0;
	void use();
	void setMVPMatrix(glm::mat4 model, glm::mat4 view, glm::mat4 projection);
	void setModelMatrix(glm::mat4 model);
	void setViewMatrix(glm::mat4 view);
	void setProjectionMatrix(glm::mat4 projection);
protected:
	virtual void initVAO() = 0;
	virtual void initShader() = 0;
	virtual void setDrawMode() = 0;
	void setShader(std::string vertexShaderPath, std::string fragmentShaderPath);

	std::shared_ptr<Shader> coreShader;
	unsigned int VAO = 0;
	unsigned int VBO = 0;
	unsigned int EBO = 0;
};

