#include "AdvanceRenderer.h"
#include "..\header\AdvanceRenderer.h"

extern float cubeVertices[];
extern unsigned int cubeVerticesSize;
extern float planeVertices[];
extern unsigned int planeVerticesSize;
using namespace std;
void AdvanceRenderer::Run()
{
	while (!glfwWindowShouldClose(window))
	{
		processInput(window);
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		frameUpdateViewProjectionMatrix();
		coreRenderPassArr["pAdvanceCubePass"]->getPassShader().setTexture("texture1", 0);
		glm::mat4 model(1.0f);
		model = glm::translate(model, glm::vec3(-1.0f, 0.0f, -1.0f));
		coreRenderPassArr["pAdvanceCubePass"]->setModelMatrix(model);
		coreRenderPassArr["pAdvanceCubePass"]->use();
		glDrawArrays(GL_TRIANGLES, 0, 36);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(2.0f, 0.0f, 0.0f));
		coreRenderPassArr["pAdvanceCubePass"]->setModelMatrix(model);
		coreRenderPassArr["pAdvanceCubePass"]->use();
		glDrawArrays(GL_TRIANGLES, 0, 36);

		coreRenderPassArr["pAdvancePlanePass"]->getPassShader().setTexture("texture1", 1);
		coreRenderPassArr["pAdvancePlanePass"]->use();
		glDrawArrays(GL_TRIANGLES, 0, 6);
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
}

void AdvanceRenderer::initResource()
{
	addVBO("CubeVBO", cubeVertices, cubeVerticesSize);
	addVBO("PlaneVBO", planeVertices, planeVerticesSize);
	addShaderProgram("AdvanceShader", "./src/GLSL/advance.vert", "./src/GLSL/advance.frag");
	addTexture("Marble", "./resources/textures/marble.jpg");
	addTexture("Metal", "./resources/textures/metal.png");
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
}
