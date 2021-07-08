#include "RPCube.h"
#include "lightStruct.h"

extern float vertices[];
extern unsigned int verticesSize;
extern glm::vec3 pointLightPositions[];

RPCube::RPCube(PassRenderer& renderCoreIn, std::string shaderName, std::string VBOName)
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
	passShader.setVec3("lightColor", lightColor);
	passShader.setTexture("material.objectDiffuse", passTextureArr["objectDiffuse"]);
	passShader.setTexture("material.objectSpecular", passTextureArr["objectSpecular"]);
	passShader.setFloat("material.shininess", 32.0f);
	
	DirLight dirLight("dirLight");
	dirLight.bindVariableToShader(passShader);
	PointLight pointLight0("pointLights[0]");
	pointLight0.bindVariableToShader(passShader);
	PointLight pointLight1("pointLights[1]");
	pointLight1.bindVariableToShader(passShader);
	PointLight pointLight2("pointLights[2]");
	pointLight2.bindVariableToShader(passShader);
	PointLight pointLight3("pointLights[3]");
	pointLight3.bindVariableToShader(passShader);
	SpotLight spotLight("spotLight");
	spotLight.bindVariableToShader(passShader);

	passShader.setVec3("pointLight[0].position", pointLightPositions[0]);
	passShader.setVec3("pointLight[1].position", pointLightPositions[1]);
	passShader.setVec3("pointLight[2].position", pointLightPositions[2]);
	passShader.setVec3("pointLight[3].position", pointLightPositions[3]);
}

void RPCube::setDrawMode()
{
	glDrawArrays(GL_TRIANGLES, 0, 36);
}
