#pragma once
#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#include "Shader.h"
#include "Texture.h"
#include <map>
#include <vector>
#include <iostream>
#include <string>
#include <vector>
class GLRenderCore;

class RenderPass
{
public:
	RenderPass(GLRenderCore& renderCoreIn, std::string shaderName, std::string VBOName);
	
	virtual void init() = 0;
	void use();
	void addPassTexture(std::string textureName);
	Shader& getPassShader() {
		return passShader;
	}
	void setModelMatrix(glm::mat4 model);
	void setViewMatrix(glm::mat4 view);
	void setProjectionMatrix(glm::mat4 projection);
private:
	GLRenderCore& renderCore;
protected:
	virtual void initVAO() = 0;
	virtual void initShader() = 0;
	virtual void setDrawMode() = 0;
protected:
	Shader& passShader;
	std::map<std::string, unsigned int> passTextureArr;

	unsigned int VAO = 0;
	unsigned int VBO = 0;
	
};

