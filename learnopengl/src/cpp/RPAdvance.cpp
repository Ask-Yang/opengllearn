#include "RPAdvance.h"
#include "..\header\RPAdvance.h"


RPAdvance::RPAdvance(PassRenderer& renderCoreIn, std::string shaderName, std::string VBOName)
	:RenderPass(renderCoreIn, shaderName, VBOName)
{
	init();
}

void RPAdvance::init()
{
	initVAO();
	initShader();
}

void RPAdvance::initVAO()
{
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER,VBO);
	
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
}

void RPAdvance::initShader()
{
	passShader.setMat4("model", glm::mat4(1.0f));
	passShader.setTexture("texture1", 0);
}

void RPAdvance::setDrawMode()
{
}
