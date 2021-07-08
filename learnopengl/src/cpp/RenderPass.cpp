#include "RenderPass.h"
#include "PassRenderer.h"
using namespace std;


RenderPass::RenderPass(PassRenderer& renderCoreIn, std::string shaderName, std::string VBOName)
	:renderCore(renderCoreIn),
	passShader(*(renderCoreIn.getShaderProgram(shaderName)))
{
	VBO = renderCore.getVBO(VBOName);
}

void RenderPass::addPassTexture(std::string textureName)
{
	passTextureArr[textureName] = renderCore.getTexture(textureName)->getTextureID();
}

void RenderPass::setModelMatrix(glm::mat4 model)
{
	passShader.setMat4("model", model);
}

void RenderPass::setViewMatrix(glm::mat4 view)
{
	passShader.setMat4("view", view);
}

void RenderPass::setProjectionMatrix(glm::mat4 projection)
{
	passShader.setMat4("projection", projection);
}

void RenderPass::use()
{
	for (auto& passTexture : passTextureArr)
		renderCore.getTexture(passTexture.first)->use();
	passShader.use();
	glBindVertexArray(VAO);
	setDrawMode();
}

