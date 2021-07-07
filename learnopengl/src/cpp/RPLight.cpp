#include "RPLight.h"

extern float vertices[];
extern unsigned int verticesSize;



RPLight::RPLight(GLRenderCore& renderCoreIn, std::string shaderName, std::string VBOName)
	:RenderPass(renderCoreIn, shaderName, VBOName)
{
	init();
}

void RPLight::init()
{
	initVAO();
	initShader();
}

void RPLight::initVAO()
{
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
}

void RPLight::initShader()
{
}

void RPLight::setDrawMode()
{
	glDrawArrays(GL_TRIANGLES, 0, 36);
}
