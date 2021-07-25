#include "ShadowRenderer.h"

using namespace std;
using namespace glm;

extern float vertices[];
extern unsigned int verticesSize;
extern float planeVertices[];
extern unsigned int planeVerticesSize;


void ShadowRenderer::Run()
{
	while (!glfwWindowShouldClose(window))
	{
		processInput(window);
		glEnable(GL_DEPTH_TEST);
		
		glViewport(0, 0, coreTexture2dArr["DepthMap"]->getWidth(), coreTexture2dArr["DepthMap"]->getHeight());
		glBindFramebuffer(GL_FRAMEBUFFER, coreFBOArr["DepthMapFBO"]);
		glClear(GL_DEPTH_BUFFER_BIT);
		coreShaderArr["ShadowShader"]->use();
		renderScene(*coreShaderArr["ShadowShader"]);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glViewport(0, 0, screenWidth, screenHeight);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);// set back
		
		glViewport(0, 0, screenWidth, screenHeight);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		frameUpdateViewProjectionMatrix(); // 会把shadow的view和projection也设置了，但是它没有这两个变量
		coreShaderArr["AdvanceLightShader"]->setVec3("viewPos", pCamera->getCameraPosition());
		for (auto& tex : coreTexture2dArr)
			tex.second->use();
		coreShaderArr["AdvanceLightShader"]->use();
		renderScene(*coreShaderArr["AdvanceLightShader"]);

		glfwSwapBuffers(window);
		glfwPollEvents(); 
	}
}

void ShadowRenderer::initScene()
{
	initVAO();
	initShaderContent();
	coreTexture2dArr["DepthMap"]->use();
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	GLfloat borderColor[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
}

void ShadowRenderer::initResource()
{
	addTexture("DepthMap", 1024, 1024, GL_DEPTH_COMPONENT);
	GLuint depthMapFBO;
	glGenFramebuffers(1, &depthMapFBO);
	glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, 
		coreTexture2dArr["DepthMap"]->getTextureID(), 0);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	coreFBOArr["DepthMapFBO"] = depthMapFBO;

	addShaderProgram("ShadowShader", "./src/GLSL/AdvanceLight/shadow.vert", "./src/GLSL/AdvanceLight/shadow.frag");
	addShaderProgram("AdvanceLightShader", "./src/GLSL/AdvanceLight/advanceLight.vert", "./src/GLSL/AdvanceLight/advanceLight.frag");

	addVBO("CubeVBO", vertices, verticesSize);
	addVBO("PlaneVBO", planeVertices, planeVerticesSize);
	addTexture("WoodTexture", "./resources/textures/wood.png");
}

void ShadowRenderer::initVAO()
{
	unsigned int cubeVAO;
	glGenVertexArrays(1, &cubeVAO);
	coreVAOArr["CubeVAO"] = cubeVAO;
	glBindBuffer(GL_ARRAY_BUFFER, coreVBOArr["CubeVBO"]);
	glBindVertexArray(cubeVAO);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3*sizeof(float)));
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6*sizeof(float)));
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	unsigned int planeVAO;
	glGenVertexArrays(1, &planeVAO);
	coreVAOArr["PlaneVAO"] = planeVAO;
	glBindVertexArray(planeVAO);
	glBindBuffer(GL_ARRAY_BUFFER, coreVBOArr["PlaneVBO"]);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void ShadowRenderer::initShaderContent()
{
	depthMapMatrix();
	coreShaderArr["AdvanceLightShader"]->setTexture("diffuseTexture", coreTexture2dArr["WoodTexture"]->getTextureSlot());
	coreShaderArr["AdvanceLightShader"]->setTexture("shadowMap", coreTexture2dArr["DepthMap"]->getTextureSlot());
	coreShaderArr["AdvanceLightShader"]->setMat4("lightSpaceMatrix", coreMatrixArr["LightSpaceMatrix"]);
	vec3 lightPos(-2.0f, 4.0f, -1.0f);
	coreShaderArr["AdvanceLightShader"]->setVec3("lightPos", lightPos);
}

void ShadowRenderer::depthMapMatrix()
{
	GLfloat near_plane = 1.0f, far_plane = 7.5f;
	mat4 lightProjection = ortho(-10.0f, 10.0f, -10.0f, 10.0f, near_plane, far_plane);
	mat4 lightView = lookAt(vec3(-2.0f, 4.0f, -1.0f), vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 1.0f, 0.0f));
	mat4 lightSpaceMatrix = lightProjection * lightView;
	coreMatrixArr["LightSpaceMatrix"] = lightSpaceMatrix;
	coreShaderArr["ShadowShader"]->setMat4("lightSpaceMatrix", lightSpaceMatrix);
	mat4 model = mat4(1.0f);
	coreShaderArr["ShadowShader"]->setMat4("model", model);
}

void ShadowRenderer::renderScene(Shader& shader)
{
	// plane
	glm::mat4 model = glm::mat4(1.0f);
	shader.setMat4("model", model);
	glBindVertexArray(coreVAOArr["PlaneVAO"]);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	// cubes
	model = glm::mat4(1.0f);
	model = glm::translate(model, vec3(0.0f, 1.5f, 0.0f));
	model = scale(model, vec3(0.5f));
	shader.setMat4("model", model);
	glBindVertexArray(coreVAOArr["CubeVAO"]);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(2.0f, 0.0f, 1.0));
	model = glm::scale(model, glm::vec3(0.5f));
	shader.setMat4("model", model);
	glBindVertexArray(coreVAOArr["CubeVAO"]);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(-1.0f, 0.0f, 2.0));
	model = glm::rotate(model, glm::radians(60.0f), glm::normalize(glm::vec3(1.0, 0.0, 1.0)));
	model = glm::scale(model, glm::vec3(0.25));
	shader.setMat4("model", model);
	glBindVertexArray(coreVAOArr["CubeVAO"]);
	glDrawArrays(GL_TRIANGLES, 0, 36);

}
