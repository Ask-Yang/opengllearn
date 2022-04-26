#include "..\..\header\Renderer\TAARenderer.h"

using namespace std;
using namespace glm;

extern float vertices[];
extern unsigned int verticesSize;
extern float planeVertices[];
extern unsigned int planeVerticesSize;
extern float quadVertices[];
extern unsigned int quadVerticesSize;

void TAARenderer::Run()
{
	while (!glfwWindowShouldClose(window))
	{
		processInput(window);
		
		Render_Taa();
		Render_WithoutJitter();
		Render_PostProcess();
		

		glfwSwapBuffers(window);
		glfwPollEvents();
	}
}

void TAARenderer::initScene()
{
	initVAO();
	initShaderContent();
	coreTexture2dArr["DepthMap"]->use();
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	GLfloat borderColor[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
}

void TAARenderer::initResource()
{
	// depth map frame buffer
	addTexture("DepthMap", 1024, 1024, GL_DEPTH_COMPONENT);// ´´½¨Ö¡»º³å¸½¼þ
	GLuint depthMapFBO;
	glGenFramebuffers(1, &depthMapFBO); // ´´½¨Ö¡»º³å
	glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D,
		coreTexture2dArr["DepthMap"]->getTextureID(), 0); // °ó¶¨Ö¡»º³å¸½¼þ 
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	coreFBOArr["DepthMapFBO"] = depthMapFBO;

	// last frame frame buffer
	GLuint lastFrameFBO;
	glGenFramebuffers(1, &lastFrameFBO);
	glBindFramebuffer(GL_FRAMEBUFFER, lastFrameFBO);

	addTexture("LastFrameBuffer", screenWidth, screenHeight, GL_RGB, GL_UNSIGNED_BYTE);
	glBindTexture(GL_TEXTURE_2D, coreTexture2dArr["LastFrameBuffer"]->getTextureID());
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D,
		coreTexture2dArr["LastFrameBuffer"]->getTextureID(), 0);
	
	GLuint lastRbo;
	glGenRenderbuffers(1, &lastRbo);
	glBindRenderbuffer(GL_RENDERBUFFER, lastRbo);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, screenWidth, screenHeight);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, lastRbo);
	
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << endl;
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	coreFBOArr["LastFrameFBO"] = lastFrameFBO;

	//InterFrameBuffer
	GLuint interFrameFBO;
	glGenFramebuffers(1, &interFrameFBO);
	glBindFramebuffer(GL_FRAMEBUFFER, interFrameFBO);

	addTexture("InterFrameBuffer", screenWidth, screenHeight, GL_RGB, GL_UNSIGNED_BYTE);
	glBindTexture(GL_TEXTURE_2D, coreTexture2dArr["InterFrameBuffer"]->getTextureID());
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D,
		coreTexture2dArr["InterFrameBuffer"]->getTextureID(), 0);
	
	GLuint interRbo;
	glGenRenderbuffers(1, &interRbo);
	glBindRenderbuffer(GL_RENDERBUFFER, interRbo);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, screenWidth, screenHeight); 
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, interRbo);
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << endl;
	coreFBOArr["InterFrameFBO"] = interFrameFBO;
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	// final frame
	GLuint combineFrameFBO;
	glGenFramebuffers(1, &combineFrameFBO);
	glBindFramebuffer(GL_FRAMEBUFFER, combineFrameFBO);

	addTexture("CombineFrameBuffer", screenWidth, screenHeight, GL_RGB, GL_UNSIGNED_BYTE);
	glBindTexture(GL_TEXTURE_2D, coreTexture2dArr["CombineFrameBuffer"]->getTextureID());
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D,
		coreTexture2dArr["CombineFrameBuffer"]->getTextureID(), 0);

	GLuint combineRbo;
	glGenRenderbuffers(1, &combineRbo);
	glBindRenderbuffer(GL_RENDERBUFFER, combineRbo);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, screenWidth, screenHeight);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, combineRbo);
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << endl;
	coreFBOArr["CombineFrameFBO"] = combineFrameFBO;
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	// add shader
	addShaderProgram("ShadowShader", "./src/GLSL/TAA/shadow.vert", "./src/GLSL/TAA/shadow.frag");
	addShaderProgram("TaaRenderShader", "./src/GLSL/TAA/TaaRender.vert", "./src/GLSL/TAA/TaaRender.frag");
	addShaderProgram("PostProcessShader", "./src/GLSL/TAA/quad.vert", "./src/GLSL/TAA/quad.frag");

	// add vbo, add model texture
	addVBO("CubeVBO", vertices, verticesSize);
	addVBO("PlaneVBO", planeVertices, planeVerticesSize);
	addVBO("QuadVBO", quadVertices, quadVerticesSize);
	addTexture("WoodTexture", "./resources/textures/wood.png");
}

void TAARenderer::initVAO()
{
	unsigned int cubeVAO;
	glGenVertexArrays(1, &cubeVAO);
	coreVAOArr["CubeVAO"] = cubeVAO;
	glBindBuffer(GL_ARRAY_BUFFER, coreVBOArr["CubeVBO"]);
	glBindVertexArray(cubeVAO);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
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

	unsigned int quadVAO;
	glGenVertexArrays(1, &quadVAO);
	coreVAOArr["QuadVAO"] = quadVAO;
	glBindVertexArray(quadVAO);
	glBindBuffer(GL_ARRAY_BUFFER, coreVBOArr["QuadVBO"]);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
	glBindVertexArray(0);
}

void TAARenderer::initShaderContent()
{
	depthMapMatrix();
	coreShaderArr["PostProcessShader"]->setTexture("screenTexture", 0);
	coreShaderArr["PostProcessShader"]->setTexture("wood", 1);
	coreShaderArr["TaaRenderShader"]->bindAndSetTexture("diffuseTexture", coreTexture2dArr["WoodTexture"]->getTextureID());
	coreShaderArr["TaaRenderShader"]->bindAndSetTexture("shadowMap", coreTexture2dArr["DepthMap"]->getTextureID());
	coreShaderArr["TaaRenderShader"]->bindAndSetTexture("lastFrame", coreTexture2dArr["LastFrameBuffer"]->getTextureID());
	coreShaderArr["TaaRenderShader"]->setMat4("lightSpaceMatrix", coreMatrixArr["LightSpaceMatrix"]);
	vec3 lightPos(-2.0f, 4.0f, -1.0f);
	coreShaderArr["TaaRenderShader"]->setVec3("lightPos", lightPos);
	coreShaderArr["TaaRenderShader"]->setFloat("alpha", 0.04);
	
}

void TAARenderer::depthMapMatrix()
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

void TAARenderer::renderScene(Shader& shader)
{
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

void TAARenderer::Render_Taa()
{
	glEnable(GL_DEPTH_TEST);
	Render_ShadowMap();
	
	glViewport(0, 0, screenWidth, screenHeight);
	glBindFramebuffer(GL_FRAMEBUFFER, coreFBOArr["InterFrameFBO"]);
	glClearColor(1, 1, 1, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	frameUpdateViewProjectionMatrix();
	JitteringProjection();
	coreShaderArr["TaaRenderShader"]->setVec3("viewPos", pCamera->getCameraPosition());
	coreShaderArr["TaaRenderShader"]->use();
	
	renderScene(*coreShaderArr["TaaRenderShader"]);
}

void TAARenderer::Render_WithoutJitter()
{
	glEnable(GL_DEPTH_TEST);
	glViewport(0, 0, screenWidth, screenHeight);
	glBindFramebuffer(GL_FRAMEBUFFER, coreFBOArr["InterFrameFBO"]);
	frameUpdateViewProjectionMatrix();
	coreShaderArr["TaaRenderShader"]->use();
	glActiveTexture(GL_TEXTURE0);
	int textureID = coreTexture2dArr["WoodTexture"]->getTextureID();
	glBindTexture(GL_TEXTURE_2D, textureID);
	glm::mat4 model;
	model = glm::mat4(1.0f);
	model = glm::translate(model, vec3(0.0f, 4.0f, 0.0f));
	model = scale(model, vec3(0.5f));
	coreShaderArr["TaaRenderShader"]->setMat4("model", model);
	glBindVertexArray(coreVAOArr["CubeVAO"]);
	glDrawArrays(GL_TRIANGLES, 0, 36);
}

void TAARenderer::Render_PostProcess()
{
	glDisable(GL_DEPTH_TEST);
	
	glViewport(0, 0, screenWidth, screenHeight);
	glBindFramebuffer(GL_FRAMEBUFFER, coreFBOArr["LastFrameFBO"]);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	coreShaderArr["PostProcessShader"]->use();
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, coreTexture2dArr["InterFrameBuffer"]->getTextureID());
	glBindVertexArray(coreVAOArr["QuadVAO"]);
	glDrawArrays(GL_TRIANGLES, 0, 6);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	coreShaderArr["PostProcessShader"]->use();
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, coreTexture2dArr["LastFrameBuffer"]->getTextureID());
	glBindVertexArray(coreVAOArr["QuadVAO"]);
	glDrawArrays(GL_TRIANGLES, 0, 6);
}

void TAARenderer::Render_ShadowMap()
{
	glViewport(0, 0, coreTexture2dArr["DepthMap"]->getWidth(), coreTexture2dArr["DepthMap"]->getHeight());
	glBindFramebuffer(GL_FRAMEBUFFER, coreFBOArr["DepthMapFBO"]);
	glClear(GL_DEPTH_BUFFER_BIT);
	coreShaderArr["ShadowShader"]->use();
	renderScene(*coreShaderArr["ShadowShader"]);
}

void TAARenderer::JitteringProjection()
{
	float SampleX = Halton[TemporalSampleIndex * 2];
	float SampleY = Halton[TemporalSampleIndex * 2 + 1];
	TemporalSampleIndex = (++TemporalSampleIndex) % 8;

	mat4 projection = perspective(radians(pCamera->getFov()), screenWidth / (float)screenHeight, 0.1f, 100.0f);
	projection[2][0] += (SampleX * 2) / screenWidth;
	projection[2][1] += (SampleY * -2) / screenHeight;
	coreShaderArr["TaaRenderShader"]->setMat4("projection", projection);
}
