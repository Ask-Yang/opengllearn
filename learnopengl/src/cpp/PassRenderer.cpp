#include "PassRenderer.h"
using namespace std;
using namespace glm;

extern float vertices[];
extern unsigned int verticesSize;
extern unsigned int indices[];
extern unsigned int indicesSize;
extern vec3 cubePositions[];
extern vec3 pointLightPositions[];

void PassRenderer::initScene()
{
	shared_ptr<RenderPass> lightPassPtr = make_shared<RPLight>(*this, "LightShader", "CubeVBO");
	shared_ptr<RenderPass> cubePassPtr = make_shared<RPCube>(*this, "CubeShader", "CubeVBO");
	cubePassPtr->addPassTexture("objectDiffuse");
	cubePassPtr->addPassTexture("objectSpecular");

	coreRenderPassArr["LightPass"] = lightPassPtr;
	coreRenderPassArr["CubePass"] = cubePassPtr;
}

void PassRenderer::initResource()
{
	addShaderProgram("CubeShader", "./src/GLSL/cube.vert", "./src/GLSL/cube.frag");
	addShaderProgram("LightShader", "./src/GLSL/cube.vert", "./src/GLSL/light.frag");
	addVBO("CubeVBO", vertices, verticesSize);
	addTexture("objectDiffuse", "./resources/textures/container2.png");
	addTexture("objectSpecular", "./resources/textures/container2_specular.png");
}

void PassRenderer::initVAO()
{
}

void PassRenderer::initShaderContent()
{
}

string PassRenderer::addTexture(string textureName, std::string texturePath)
{
	coreTexture2dArr[textureName] = make_shared<Texture2D>(texturePath);
	return textureName;
}

std::string PassRenderer::addTexture(std::string textureName, unsigned int width, unsigned int height, unsigned int GLTEXTUREFORMAT)
{
	coreTexture2dArr[textureName] = make_shared<Texture2D>(width, height, GLTEXTUREFORMAT);
	return textureName;
}

std::string PassRenderer::addVBO(std::string VBOName, float* vertices, unsigned int verticesSize)
{
	unsigned int VBO;
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, verticesSize, vertices, GL_STATIC_DRAW);
	coreVBOArr[VBOName] = VBO;
	return VBOName;
}

void PassRenderer::Run()
{
	while (!glfwWindowShouldClose(window))
	{
		processInput(window);
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		frameUpdateViewProjectionMatrix();
		coreRenderPassArr["CubePass"]->getPassShader().setVec3("viewPos", pCamera->getCameraPosition());
		coreRenderPassArr["CubePass"]->getPassShader().setVec3("spotLight.position", pCamera->getCameraPosition());
		coreRenderPassArr["CubePass"]->getPassShader().setVec3("spotLight.direction", pCamera->getCameraFront());
		for (int i = 0; i < 10; i++)
		{
			mat4 model(1.0f);
			model = translate(model, cubePositions[i]);
			float angle = 20.0f * i;
			model = rotate(model, radians(angle), vec3(1.0f, 0.3f, 0.5f));
			mat4 normalTransformMatrix = transpose(inverse(model));
			coreRenderPassArr["CubePass"]->getPassShader().setMat4("normal", normalTransformMatrix);
			coreRenderPassArr["CubePass"]->getPassShader().setMat4("model", model);
			coreRenderPassArr["CubePass"]->use();
		}
		for (int i = 0; i < 4; i++)
		{
			mat4 model(1.0f);
			model = translate(model, pointLightPositions[i]);
			model = scale(model, vec3(0.2f));
			coreRenderPassArr["LightPass"]->getPassShader().setMat4("model", model);
			coreRenderPassArr["LightPass"]->use();
		}

		glfwSwapBuffers(window);
		glfwPollEvents();
	}
}

void PassRenderer::enableTexture(std::string textureName)
{
	coreTexture2dArr[textureName]->use();
}