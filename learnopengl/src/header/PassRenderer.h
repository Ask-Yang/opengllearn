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

	std::string addTexture(std::string textureName, std::string texturePath);

	std::string addVBO(std::string VBOName, float* vertices, unsigned int verticesSize);
	void currentFrameObjectUpdate();


protected:
	std::map<std::string, unsigned int> coreVBOArr;
	std::map<std::string, std::shared_ptr<Texture2D>> coreTexture2dArr;
	std::map<std::string, std::shared_ptr<RenderPass>> coreRenderPassArr;
};

