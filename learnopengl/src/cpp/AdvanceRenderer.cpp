#include "AdvanceRenderer.h"
#include "..\header\AdvanceRenderer.h"

extern float cubeVertices[];
extern unsigned int cubeVerticesSize;
extern float planeVertices[];
extern unsigned int planeVerticesSize;
extern float transparentVertices[];
extern unsigned int transparentVerticesSize;
extern float quadVertices[];
extern unsigned int quadVerticesSize;
extern float skyboxVertices[];
extern unsigned int skyboxVerticesSize;

using namespace std;
using namespace glm;
vector<glm::vec3> windowPos;

void AdvanceRenderer::Run()
{
	//while (!glfwWindowShouldClose(window))
	//{
	//	processInput(window);
	//	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	//	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//	frameUpdateViewProjectionMatrix();

	//	glfwSwapBuffers(window);
	//	glfwPollEvents();
	//}

	SkyboxRun();
}

void AdvanceRenderer::initResource()
{
	addVBO("CubeVBO", cubeVertices, cubeVerticesSize);
	addVBO("PlaneVBO", planeVertices, planeVerticesSize);
	addShaderProgram("AdvanceShader", "./src/GLSL/advance.vert", "./src/GLSL/advance.frag");
	addTexture("Marble", "./resources/textures/container.jpg");
	addTexture("Metal", "./resources/textures/metal.png");

	// stencil
	addShaderProgram("ColorShader", "./src/GLSL/advance.vert", "./src/GLSL/outline.frag");
	// blend
	addVBO("RectangleVBO", transparentVertices, transparentVerticesSize);
	addShaderProgram("BlendShader", "./src/GLSL/advance.vert", "./src/GLSL/transparent.frag");
	addTexture("Grass", "./resources/textures/window.png");
	// frame buffer
	addVBO("QuadVBO", quadVertices, quadVerticesSize);
	addShaderProgram("PostProcessShader", "./src/GLSL/postprocess.vert", "./src/GLSL/postprocess.frag");
	customizeFrameBuffer();
	// cubemap(skybox)
	loadCubemap();
	addVBO("SkyboxVBO", skyboxVertices, skyboxVerticesSize);
	addShaderProgram("SkyboxShader", "./src/GLSL/skybox.vert", "./src/GLSL/skybox.frag");
}

void AdvanceRenderer::initScene()
{
	coreTexture2dArr["Marble"]->setTextureWrapMode(Texture2D::WrapMode::Repeat);
	coreTexture2dArr["Metal"]->setTextureWrapMode(Texture2D::WrapMode::Repeat);

	shared_ptr<RenderPass> pAdvanceCubePass = make_shared<RPAdvance>(*this, "AdvanceShader", "CubeVBO");
	shared_ptr<RenderPass> pAdvancePlanePass = make_shared<RPAdvance>(*this, "AdvanceShader", "PlaneVBO");
	pAdvanceCubePass->addPassTexture("Marble");
	pAdvancePlanePass->addPassTexture("Metal");
	coreRenderPassArr["pAdvanceCubePass"] = pAdvanceCubePass;
	coreRenderPassArr["pAdvancePlanePass"] = pAdvancePlanePass;

	// stencil
	shared_ptr<RenderPass> pColorPass = make_shared<RPAdvance>(*this, "ColorShader", "CubeVBO");
	coreRenderPassArr["pColorPass"] = pColorPass;
	// blend
	coreTexture2dArr["Grass"]->setTextureWrapMode(Texture2D::WrapMode::Clamp);
	shared_ptr<RenderPass> pBlendPass = make_shared<RPAdvance>(*this, "BlendShader", "RectangleVBO");
	coreRenderPassArr["pBlendPass"] = pBlendPass;
	pBlendPass->addPassTexture("Grass");
	coreShaderArr["BlendShader"]->setTexture("texture1", coreTexture2dArr["Grass"]->getTextureSlot());
	windowPos.push_back(glm::vec3(-1.5f, 0.0f, -0.48f));
	windowPos.push_back(glm::vec3(1.5f, 0.0f, 0.51f));
	windowPos.push_back(glm::vec3(0.0f, 0.0f, 0.7f));
	windowPos.push_back(glm::vec3(-0.3f, 0.0f, -2.3f));
	windowPos.push_back(glm::vec3(0.5f, 0.0f, -0.6f));
	// frame buffer(postprocess)
	shared_ptr<RenderPass> pPostProcessPass = make_shared<RPPostProcess>(*this, "PostProcessShader", "QuadVBO");
	coreRenderPassArr["pPostProcessPass"] = pPostProcessPass;
	// cubemap(skybox) 
	shared_ptr<RenderPass> pSkyboxPass = make_shared<RPSkybox>(*this, "SkyboxShader", "SkyboxVBO");
	coreRenderPassArr["pSkyboxPass"] = pSkyboxPass;
}

void AdvanceRenderer::loadCubemap()
{
	vector<string> faces{
		"./resources/textures/skybox/right.jpg",
		"./resources/textures/skybox/left.jpg",
		"./resources/textures/skybox/top.jpg",
		"./resources/textures/skybox/bottom.jpg",
		"./resources/textures/skybox/front.jpg",
		"./resources/textures/skybox/back.jpg"
	};
	glGenTextures(1, &cubemap);
	glBindTexture(GL_TEXTURE_CUBE_MAP, cubemap);
	int width, height, nrChannels;
	for (unsigned int i = 0; i < faces.size(); i++)
	{
		unsigned char* data = stbi_load(faces[i].c_str(), &width, &height, &nrChannels, 0);
		if (data)
		{
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
				0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
			stbi_image_free(data);
		}
		else
		{
			std::cout << "Cubemap texture failed to load at path: " << faces[i] << std::endl;
			stbi_image_free(data);
		}
	}
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
}

void AdvanceRenderer::customizeFrameBuffer()
{
	glGenFramebuffers(1, &framebuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);

	glGenTextures(1, &texColorBuffer);
	glBindTexture(GL_TEXTURE_2D, texColorBuffer);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, screenWidth, screenHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glBindTexture(GL_TEXTURE_2D, 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texColorBuffer, 0);

	unsigned int rbo;
	glGenRenderbuffers(1, &rbo);
	glBindRenderbuffer(GL_RENDERBUFFER, rbo);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, screenWidth, screenHeight);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0); // 解绑帧缓冲，防止程序其它地方错误地渲染这个帧缓冲
}

void AdvanceRenderer::drawScene()
{
	coreRenderPassArr["pAdvancePlanePass"]->getPassShader().setTexture("texture1", 1);
	coreRenderPassArr["pAdvancePlanePass"]->use();
	glDrawArrays(GL_TRIANGLES, 0, 6);

	glm::mat4 model(1.0f);
	model = glm::translate(glm::mat4(1.0f), glm::vec3(-1.0f, 0.0f, -1.0f));
	coreRenderPassArr["pAdvanceCubePass"]->getPassShader().setTexture("texture1", 0);
	coreRenderPassArr["pAdvanceCubePass"]->setModelMatrix(model);
	coreRenderPassArr["pAdvanceCubePass"]->use();
	glDrawArrays(GL_TRIANGLES, 0, 36);

	model = glm::translate(glm::mat4(1.0f), glm::vec3(2.0f, 0.0f, 0.0f));
	coreRenderPassArr["pAdvanceCubePass"]->setModelMatrix(model);
	coreRenderPassArr["pAdvanceCubePass"]->use();
	glDrawArrays(GL_TRIANGLES, 0, 36);
}

void AdvanceRenderer::StencilRun()
{
	while (!glfwWindowShouldClose(window))
	{
		processInput(window);
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

		frameUpdateViewProjectionMatrix();
		coreRenderPassArr["pAdvancePlanePass"]->getPassShader().setTexture("texture1", 1);
		coreRenderPassArr["pAdvancePlanePass"]->use();
		glDrawArrays(GL_TRIANGLES, 0, 6);

		glEnable(GL_STENCIL_TEST);
		glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);

		glStencilFunc(GL_ALWAYS, 1, 0xFF);
		glStencilMask(0xFF);
		coreRenderPassArr["pAdvanceCubePass"]->getPassShader().setTexture("texture1", 0);
		glm::mat4 model(1.0f);
		model = glm::translate(glm::mat4(1.0f), glm::vec3(-1.0f, 0.0f, -1.0f));
		coreRenderPassArr["pAdvanceCubePass"]->setModelMatrix(model);
		coreRenderPassArr["pAdvanceCubePass"]->use();
		glDrawArrays(GL_TRIANGLES, 0, 36);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(2.0f, 0.0f, 0.0f));
		coreRenderPassArr["pAdvanceCubePass"]->setModelMatrix(model);
		coreRenderPassArr["pAdvanceCubePass"]->use();
		glDrawArrays(GL_TRIANGLES, 0, 36);

		glStencilFunc(GL_NOTEQUAL, 1, 0xFF); // ref 是个 int
		glStencilMask(0x00);
		glDisable(GL_DEPTH_TEST);
		model = glm::translate(glm::mat4(1.0f), glm::vec3(-1.0f, 0.0f, -1.0f));
		model = glm::scale(model, glm::vec3(1.1f));
		coreRenderPassArr["pColorPass"]->setModelMatrix(model);
		coreRenderPassArr["pColorPass"]->use();
		glDrawArrays(GL_TRIANGLES, 0, 36);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(2.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.1f));
		coreRenderPassArr["pColorPass"]->setModelMatrix(model);
		coreRenderPassArr["pColorPass"]->use();
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glStencilMask(0xFF);
		glEnable(GL_DEPTH_TEST);
		glDisable(GL_STENCIL_TEST);


		glfwSwapBuffers(window);
		glfwPollEvents();
	}
}

void AdvanceRenderer::TransparencyRun()
{
	glEnable(GL_CULL_FACE);
	while (!glfwWindowShouldClose(window))
	{
		map<float, glm::vec3> sorted;
		for (unsigned int i = 0; i < windowPos.size(); i++)
		{
			float distance = glm::distance(windowPos[i], pCamera->getCameraPosition());
			sorted[distance] = windowPos[i];
		}
		processInput(window);
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
		frameUpdateViewProjectionMatrix();
		drawScene();

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC1_ALPHA);
		for (auto p = sorted.rbegin();p!=sorted.rend();p++)
		{
			glm::mat4 model = glm::mat4(1.0f);
			model = glm::translate(glm::mat4(1.0f), p->second);
			coreRenderPassArr["pBlendPass"]->setModelMatrix(model);
			coreRenderPassArr["pBlendPass"]->use();
			glDrawArrays(GL_TRIANGLES, 0, 6);
		}
		glDisable(GL_BLEND);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}
}

void AdvanceRenderer::FrameBufferRun()
{
	while (!glfwWindowShouldClose(window))
	{
		processInput(window);
		glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glEnable(GL_DEPTH_TEST);
		frameUpdateViewProjectionMatrix();
		drawScene();

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glDisable(GL_DEPTH_TEST);
		glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texColorBuffer);
		coreRenderPassArr["pPostProcessPass"]->use();

		glfwSwapBuffers(window);
		glfwPollEvents();
	}
}

void AdvanceRenderer::SkyboxRun()
{
	while (!glfwWindowShouldClose(window))
	{
		processInput(window);

		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glEnable(GL_DEPTH_TEST);

		frameUpdateViewProjectionMatrix();
		mat4 skyboxView = mat4(mat3x3(pCamera->getViewMatrix()));
		coreShaderArr["SkyboxShader"]->setMat4("view", skyboxView);
		
		drawScene();

		glDepthFunc(GL_LEQUAL);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_CUBE_MAP, cubemap);
		coreRenderPassArr["pSkyboxPass"]->use();
		glDepthFunc(GL_LESS);



		glfwSwapBuffers(window);
		glfwPollEvents();
	}

}
