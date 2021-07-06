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

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
}

void RPCube::initShader()
{
	glm::vec3 lightColor;
	lightColor.x = sin(glfwGetTime() * 2.0f);
	lightColor.y = sin(glfwGetTime() * 0.7f);
	lightColor.z = sin(glfwGetTime() * 1.3f);

	coreShader->setMat4("model", glm::mat4(1.0f));

	coreShader->setVec3("material.objectAmbient", 1.0f, 0.5f, 0.31f);
	coreShader->setVec3("material.objectDiffuse", 1.0f, 0.5f, 0.31f);
	coreShader->setVec3("material.objectSpecular", 0.5f, 0.5f, 0.5f);
	coreShader->setFloat("material.shininess", 32.0f);

	coreShader->setVec3("lightColor", lightColor);
	coreShader->setVec3("light.position", -1.2f, 1.0f, 2.0f);
	coreShader->setVec3("light.ambientStrength", 0.2f, 0.2f, 0.2f);
	coreShader->setVec3("light.diffuseStrength", 0.5f, 0.5f, 0.5f); // 将光照调暗了一些以搭配场景
	coreShader->setVec3("light.specularStrength", 1.0f, 1.0f, 1.0f);
}

void RPCube::setDrawMode()
{
	glDrawArrays(GL_TRIANGLES, 0, 36);
}
