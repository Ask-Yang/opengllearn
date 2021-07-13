#include "RPSkybox.h"

RPSkybox::RPSkybox(PassRenderer& renderCoreIn, std::string shaderName, std::string VBOName)
	:RenderPass(renderCoreIn, shaderName, VBOName)
{
	init();
}

void RPSkybox::init()
{
	initVAO();
	initShader();
}

void RPSkybox::initVAO()
{
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
}

void RPSkybox::initShader()
{
	passShader.setTexture("skybox", 0);
}

void RPSkybox::setDrawMode()
{
	glDrawArrays(GL_TRIANGLES, 0, 36);
}
