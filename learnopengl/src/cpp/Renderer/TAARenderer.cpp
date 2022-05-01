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
		
		Render_Lighting();
		Render_Velocity();
		Render_Taa();
		Render_WithoutJitter();
		Render_PostProcess();
		
		RecordMatrix();
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

	// velocity frame
	GLuint velocityFrameFBO;
	glGenFramebuffers(1, &velocityFrameFBO);
	glBindFramebuffer(GL_FRAMEBUFFER, velocityFrameFBO);

	addTexture("VelocityFrameBuffer", screenWidth, screenHeight, GL_RGB, GL_UNSIGNED_BYTE);
	glBindTexture(GL_TEXTURE_2D, coreTexture2dArr["VelocityFrameBuffer"]->getTextureID());
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D,
		coreTexture2dArr["VelocityFrameBuffer"]->getTextureID(), 0);

	GLuint velocityRbo;
	glGenRenderbuffers(1, &velocityRbo);
	glBindRenderbuffer(GL_RENDERBUFFER, velocityRbo);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, screenWidth, screenHeight);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, velocityRbo);
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << endl;
	coreFBOArr["VelocityFrameFBO"] = velocityFrameFBO;
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	// current frame
	GLuint currentFrameFBO;
	glGenFramebuffers(1, &currentFrameFBO);
	glBindFramebuffer(GL_FRAMEBUFFER, currentFrameFBO);

	addTexture("CurrentFrameBuffer", screenWidth, screenHeight, GL_RGB, GL_UNSIGNED_BYTE);
	glBindTexture(GL_TEXTURE_2D, coreTexture2dArr["CurrentFrameBuffer"]->getTextureID());
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D,
		coreTexture2dArr["CurrentFrameBuffer"]->getTextureID(), 0);

	GLuint currentRbo;
	glGenRenderbuffers(1, &currentRbo);
	glBindRenderbuffer(GL_RENDERBUFFER, currentRbo);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, screenWidth, screenHeight);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, currentRbo);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << endl;
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	coreFBOArr["CurrentFrameFBO"] = currentFrameFBO;

	// add shader
	addShaderProgram("ShadowShader", "./src/GLSL/TAA/shadow.vert", "./src/GLSL/TAA/shadow.frag");
	addShaderProgram("TaaRenderShader", "./src/GLSL/TAA/TaaRenderAdvance.vert", "./src/GLSL/TAA/TaaRenderAdvance.frag");
	addShaderProgram("QuadShader", "./src/GLSL/TAA/quad.vert", "./src/GLSL/TAA/quad.frag");
	addShaderProgram("VelocityShader", "./src/GLSL/TAA/velocity.vert", "./src/GLSL/TAA/velocity.frag");
	addShaderProgram("LightingShader", "./src/GLSL/TAA/lighting.vert", "./src/GLSL/TAA/lighting.frag");

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
	coreShaderArr["QuadShader"]->setTexture("screenTexture", 0);
	coreShaderArr["QuadShader"]->setTexture("wood", 1);

	coreShaderArr["TaaRenderShader"]->bindAndSetTexture("lastFrame", coreTexture2dArr["LastFrameBuffer"]->getTextureID());
	coreShaderArr["TaaRenderShader"]->bindAndSetTexture("velocityBuffer", coreTexture2dArr["VelocityFrameBuffer"]->getTextureID());
	coreShaderArr["TaaRenderShader"]->bindAndSetTexture("currentFrame", coreTexture2dArr["CurrentFrameBuffer"]->getTextureID());
	coreShaderArr["TaaRenderShader"]->setFloat("alpha", 0.04);

	coreShaderArr["LightingShader"]->bindAndSetTexture("diffuseTexture", coreTexture2dArr["WoodTexture"]->getTextureID());
	coreShaderArr["LightingShader"]->bindAndSetTexture("shadowMap", coreTexture2dArr["DepthMap"]->getTextureID());
	coreShaderArr["LightingShader"]->setMat4("lightSpaceMatrix", coreMatrixArr["LightSpaceMatrix"]);
	vec3 lightPos(-2.0f, 4.0f, -1.0f);
	coreShaderArr["LightingShader"]->setVec3("lightPos", lightPos);
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
	int modelMatrixIndex = 0;
	glm::mat4 model = glm::mat4(1.0f);
	shader.setMat4("model", model);
	currentModelMatrixMap[modelMatrixIndex++] = model;
	if (!lastModelMatrixQueue.empty())
	{
		shader.setMat4("lastModel", lastModelMatrixQueue.front());
		lastModelMatrixQueue.pop();
	}
	glBindVertexArray(coreVAOArr["PlaneVAO"]);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	// cubes
	model = glm::mat4(1.0f);
	model = glm::translate(model, vec3(0.0f, 1.5f, 0.0f));
	model = scale(model, vec3(0.5f));
	shader.setMat4("model", model);
	currentModelMatrixMap[modelMatrixIndex++] = model;
	if (!lastModelMatrixQueue.empty())
	{
		shader.setMat4("lastModel", lastModelMatrixQueue.front());
		lastModelMatrixQueue.pop();
	}
	glBindVertexArray(coreVAOArr["CubeVAO"]);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(2.0f, 0.0f, 1.0));
	model = glm::scale(model, glm::vec3(0.5f));
	shader.setMat4("model", model);
	currentModelMatrixMap[modelMatrixIndex++] = model;
	if (!lastModelMatrixQueue.empty())
	{
		shader.setMat4("lastModel", lastModelMatrixQueue.front());
		lastModelMatrixQueue.pop();
	}
	glBindVertexArray(coreVAOArr["CubeVAO"]);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(-1.0f, 0.0f, 2.0));
	model = glm::rotate(model, glm::radians(60.0f), glm::normalize(glm::vec3(1.0, 0.0, 1.0)));
	model = glm::scale(model, glm::vec3(0.25));
	shader.setMat4("model", model);
	currentModelMatrixMap[modelMatrixIndex++] = model;
	if (!lastModelMatrixQueue.empty())
	{
		shader.setMat4("lastModel", lastModelMatrixQueue.front());
		lastModelMatrixQueue.pop();
	}
	glBindVertexArray(coreVAOArr["CubeVAO"]);
	glDrawArrays(GL_TRIANGLES, 0, 36);
}

void TAARenderer::RecordMatrix()
{
	mat4 view = pCamera->getViewMatrix();
	mat4 projection = perspective(radians(pCamera->getFov()), screenWidth / (float)screenHeight, 0.1f, 100.0f);
	lastView = view;
	lastProjection = projection;
	for (auto& m : currentModelMatrixMap)
		lastModelMatrixQueue.push(m.second);
}

void TAARenderer::Render_Lighting()
{
	glEnable(GL_DEPTH_TEST);
	Render_ShadowMap();
	// lighting render
	glViewport(0, 0, screenWidth, screenHeight);
	glBindFramebuffer(GL_FRAMEBUFFER, coreFBOArr["CurrentFrameFBO"]);
	glClearColor(1, 1, 1, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	frameUpdateViewProjectionMatrix();
	JitteringProjection();
	coreShaderArr["LightingShader"]->setVec3("viewPos", pCamera->getCameraPosition());
	coreShaderArr["LightingShader"]->use();
	renderScene(*coreShaderArr["LightingShader"]);
}

void TAARenderer::Render_Velocity()
{
	glEnable(GL_DEPTH_TEST);
	glViewport(0, 0, screenWidth, screenHeight);
	glBindFramebuffer(GL_FRAMEBUFFER, coreFBOArr["VelocityFrameFBO"]);
	glClearColor(0, 0, 0, 0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	frameUpdateViewProjectionMatrix();
	coreShaderArr["VelocityShader"]->setMat4("lastProjection", lastProjection);
	coreShaderArr["VelocityShader"]->setMat4("lastView", lastView);
	coreShaderArr["VelocityShader"]->use();

	renderScene(*coreShaderArr["VelocityShader"]);
}

void TAARenderer::Render_Taa()
{
	glDisable(GL_DEPTH_TEST);
	glViewport(0, 0, screenWidth, screenHeight);
	glBindFramebuffer(GL_FRAMEBUFFER, coreFBOArr["InterFrameFBO"]);
	glClearColor(0,0,0,0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	coreShaderArr["TaaRenderShader"]->use();
	glBindVertexArray(coreVAOArr["QuadVAO"]);
	glDrawArrays(GL_TRIANGLES, 0, 6);

	// render to last frame
	glDisable(GL_DEPTH_TEST);
	glViewport(0, 0, screenWidth, screenHeight);
	glBindFramebuffer(GL_FRAMEBUFFER, coreFBOArr["LastFrameFBO"]);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	coreShaderArr["QuadShader"]->use();
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, coreTexture2dArr["InterFrameBuffer"]->getTextureID());
	glBindVertexArray(coreVAOArr["QuadVAO"]);
	glDrawArrays(GL_TRIANGLES, 0, 6);
}

void TAARenderer::Render_WithoutJitter()
{
	//render unjitter object
	glEnable(GL_DEPTH_TEST);
	glViewport(0, 0, screenWidth, screenHeight);
	glBindFramebuffer(GL_FRAMEBUFFER, coreFBOArr["InterFrameFBO"]);
	frameUpdateViewProjectionMatrix();
	coreShaderArr["LightingShader"]->use();
	glm::mat4 model;
	model = glm::mat4(1.0f);
	model = glm::translate(model, vec3(0.0f, 4.0f, 0.0f));
	model = scale(model, vec3(0.5f));
	coreShaderArr["LightingShader"]->setMat4("model", model);
	glBindVertexArray(coreVAOArr["CubeVAO"]);
	glDrawArrays(GL_TRIANGLES, 0, 36);
}

void TAARenderer::Render_PostProcess()
{
	glDisable(GL_DEPTH_TEST);
	// render to screen
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	coreShaderArr["QuadShader"]->use();
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, coreTexture2dArr["InterFrameBuffer"]->getTextureID());
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
