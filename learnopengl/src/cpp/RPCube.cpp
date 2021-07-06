#include "RPCube.h"

extern float vertices[];
extern unsigned int verticesSize;


RPCube::RPCube(GLRenderCore& renderCoreIn, std::string shaderName, std::string VBOName)
	:RenderPass(renderCoreIn, shaderName, VBOName)
{
	init();
}

void RPCube::init()
{
	initVAO();
	initShader();
}

void RPCube::initVAO()
{
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);
}

void RPCube::initShader()
{
	glm::vec3 lightColor(1,1,1);

	passShader.setMat4("model", glm::mat4(1.0f));

	passShader.setTexture("material.objectDiffuse", passTextureArr["objectDiffuse"]);
	passShader.setTexture("material.objectSpecular", passTextureArr["objectSpecular"]);

	passShader.setFloat("material.shininess", 32.0f);

	passShader.setVec3("lightColor", lightColor);
	passShader.setVec3("light.position", -1.2f, 1.0f, 2.0f);
	passShader.setVec3("light.ambientStrength", 0.2f, 0.2f, 0.2f);
	passShader.setVec3("light.diffuseStrength", 0.5f, 0.5f, 0.5f); 
	passShader.setVec3("light.specularStrength", 1.0f, 1.0f, 1.0f);
}

void RPCube::setDrawMode()
{
	glDrawArrays(GL_TRIANGLES, 0, 36);
}
