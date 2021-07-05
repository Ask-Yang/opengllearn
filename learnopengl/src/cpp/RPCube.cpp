#include "RPCube.h"
extern float vertices[];
extern unsigned int verticesSize;

RPCube::RPCube()
{
	init();
}

void RPCube::init()
{
	setShader("./src/GLSL/vShader.vert", "./src/GLSL/fShader.frag");
	initVAO();
	initShader();
}

void RPCube::initVAO()
{
	glGenBuffers(1, &VBO);
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, verticesSize, vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
}

void RPCube::initShader()
{
	coreShader->setVec3("objectColor", 1.0f, 0.5f, 0.31f);
	coreShader->setVec3("lightColor", 1.0f, 1.0f, 1.0f);
	coreShader->setMat4("model", glm::mat4(1.0f));
}

void RPCube::setDrawMode()
{
	glDrawArrays(GL_TRIANGLES, 0, 36);
}
