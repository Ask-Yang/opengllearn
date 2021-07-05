#include "RPLight.h"

extern float vertices[];
extern unsigned int verticesSize;

RPLight::RPLight()
{
	init();
}

void RPLight::init()
{
	setShader("./src/GLSL/vShader.vert", "./src/GLSL/light.frag");
	initVAO();
	initShader();
}

void RPLight::initVAO()
{
	glGenBuffers(1, &VBO);
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, verticesSize, vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
}

void RPLight::initShader()
{
	glm::vec3 lightPos(1.2f, 1.0f, 2.0f);
	glm::mat4 model(1.0f);
	model = glm::translate(model, lightPos);
	model = glm::scale(model, glm::vec3(0.2f));
	coreShader->setMat4("model", model);
}

void RPLight::setDrawMode()
{
	glDrawArrays(GL_TRIANGLES, 0, 36);
}
