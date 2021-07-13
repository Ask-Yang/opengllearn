#include "RPPostProcess.h"
#include "..\header\RPPostProcess.h"

RPPostProcess::RPPostProcess(PassRenderer& renderCoreIn, std::string shaderName, std::string VBOName)
	:RenderPass(renderCoreIn, shaderName, VBOName)
{
	init();
}

void RPPostProcess::init()
{
	initVAO();
	initShader();
}

void RPPostProcess::initVAO()
{
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER,VBO);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
	
}

void RPPostProcess::initShader()
{
	passShader.setTexture("screenTexture", 0);
}

void RPPostProcess::setDrawMode()
{
	glDrawArrays(GL_TRIANGLES, 0, 6);
}
