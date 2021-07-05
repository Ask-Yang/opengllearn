#include "RenderPass.h"
using namespace std;

void RenderPass::setShader(std::string vertexShaderPath, std::string fragmentShaderPath)
{
	coreShader = make_shared<Shader>(vertexShaderPath, fragmentShaderPath);
}

void RenderPass::use()
{
	coreShader->use();
	glBindVertexArray(VAO);
	setDrawMode();
}

void RenderPass::setMVPMatrix(glm::mat4 model, glm::mat4 view, glm::mat4 projection)
{
	coreShader->setMat4("model", model);
	coreShader->setMat4("view", view);
	coreShader->setMat4("projection", projection);
}

void RenderPass::setModelMatrix(glm::mat4 model)
{
	coreShader->setMat4("model", model);
}

void RenderPass::setViewMatrix(glm::mat4 view)
{
	coreShader->setMat4("view", view);
}

void RenderPass::setProjectionMatrix(glm::mat4 projection)
{
	coreShader->setMat4("projection", projection);
}
