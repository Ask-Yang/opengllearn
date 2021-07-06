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
	glm::vec3 lightPos(-1.2f, 1.0f, 2.0f);
	glm::mat4 model(1.0f);
	model = glm::translate(model, lightPos);
	model = glm::scale(model, glm::vec3(0.2f));
	passShader.setMat4("model", model);
}

void RPLight::setDrawMode()
{
	glDrawArrays(GL_TRIANGLES, 0, 36);
}
