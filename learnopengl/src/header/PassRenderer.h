#pragma once
#include "GLRenderCore.h"
class PassRenderer :
    public GLRenderCore
{
public:
	virtual void Run() override;
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
protected:
	virtual void initScene() override;
	virtual void initResource() override;
	virtual void initVAO();
	virtual void initShaderContent();

	std::string addTexture(std::string textureName, std::string texturePath);
	std::string addTexture(std::string textureName, unsigned int width, unsigned int height, unsigned int GLTEXTUREFORMAT);
	std::string addVBO(std::string VBOName, float* vertices, unsigned int verticesSize);

protected:
	std::map<std::string, glm::mat4> coreMatrixArr;
	std::map<std::string, unsigned int> coreFBOArr;
	std::map<std::string, unsigned int> coreVAOArr;
	std::map<std::string, unsigned int> coreVBOArr;
	std::map<std::string, std::shared_ptr<Texture2D>> coreTexture2dArr;
	std::map<std::string, std::shared_ptr<RenderPass>> coreRenderPassArr;
};

