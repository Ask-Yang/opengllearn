#include "ModelRenderer.h"

using namespace std;
using namespace glm;
void ModelRenderer::Run()
{
	while (!glfwWindowShouldClose(window))
	{
		processInput(window);
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		currentFrameObjectUpdate();
		model->use(*coreShaderArr["modelShader"]);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}
}

void ModelRenderer::initResource()
{
	addShaderProgram("modelShader", "./src/GLSL/model.vert", "./src/GLSL/model.frag");
	model = make_shared<Model>("./resources/objects/backpack/backpack.obj");
}

void ModelRenderer::initScene()
{
	glm::mat4 model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f)); // translate it down so it's at the center of the scene
	model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));	// it's a bit too big for our scene, so scale it down
	coreShaderArr["modelShader"]->setMat4("model", model);
}


void ModelRenderer::currentFrameObjectUpdate()
{
	frameUpdateViewProjectionMatrix();
}
