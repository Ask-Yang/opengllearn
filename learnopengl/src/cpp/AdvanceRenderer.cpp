#include "AdvanceRenderer.h"
#include "..\header\AdvanceRenderer.h"

extern float cubeVertices[];
extern unsigned int cubeVerticesSize;
extern float planeVertices[];
extern unsigned int planeVerticesSize;
extern float transparentVertices[];
extern unsigned int transparentVerticesSize;

using namespace std;
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

	TransparencyRun();
}

void AdvanceRenderer::initResource()
{
	addVBO("CubeVBO", cubeVertices, cubeVerticesSize);
	addVBO("PlaneVBO", planeVertices, planeVerticesSize);
	addShaderProgram("AdvanceShader", "./src/GLSL/advance.vert", "./src/GLSL/advance.frag");
	addTexture("Marble", "./resources/textures/marble.jpg");
	addTexture("Metal", "./resources/textures/metal.png");

	// stencil
	addShaderProgram("ColorShader", "./src/GLSL/advance.vert", "./src/GLSL/outline.frag");
	//blend
	addVBO("RectangleVBO", transparentVertices, transparentVerticesSize);
	addShaderProgram("BlendShader", "./src/GLSL/advance.vert", "./src/GLSL/transparent.frag");
	addTexture("Grass", "./resources/textures/window.png");
}

void AdvanceRenderer::initScene()
{
	coreTexture2dArr["Marble"]->setTextureWrapMode(Texture2D::WrapMode::Repeat);
	coreTexture2dArr["Metal"]->setTextureWrapMode(Texture2D::WrapMode::Repeat);
	coreTexture2dArr["Grass"]->setTextureWrapMode(Texture2D::WrapMode::Clamp);
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
	shared_ptr<RenderPass> pBlendPass = make_shared<RPAdvance>(*this, "BlendShader", "RectangleVBO");
	coreRenderPassArr["pBlendPass"] = pBlendPass;
	pBlendPass->addPassTexture("Grass");
	coreShaderArr["BlendShader"]->setTexture("texture1", coreTexture2dArr["Grass"]->getTextureSlot());
	windowPos.push_back(glm::vec3(-1.5f, 0.0f, -0.48f));
	windowPos.push_back(glm::vec3(1.5f, 0.0f, 0.51f));
	windowPos.push_back(glm::vec3(0.0f, 0.0f, 0.7f));
	windowPos.push_back(glm::vec3(-0.3f, 0.0f, -2.3f));
	windowPos.push_back(glm::vec3(0.5f, 0.0f, -0.6f));

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

		glStencilFunc(GL_NOTEQUAL, 1, 0xFF); // ref ÊÇ¸ö int
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
