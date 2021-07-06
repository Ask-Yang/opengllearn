#include "GLRenderCore.h"

using namespace std;
using namespace glm;

extern float vertices[];
extern unsigned int verticesSize;
extern unsigned int indices[];
extern unsigned int indicesSize;
extern vec3 cubePositions[];

static Camera camera(vec3(4, 0, 4), vec3(0, 0, 0), vec3(0, 1, 0)); // 那如果有多个窗口如何实现呢？
// 可能的实现：每个类保存自己的这个对应的变量，然后要用的时候就取这个全局变量的值，进行计算，看情况进行覆写
static bool sg_FirstMouse = true;
static float sg_lastFrameTime = 0.0f;
static float sg_lastMouseX = 0.0f;
static float sg_lastMouseY = 0.0f;

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}
void mouse_callback(GLFWwindow* window, double xpos, double ypos) 
{
	if (sg_FirstMouse)
	{
		sg_lastMouseX = xpos;
		sg_lastMouseY = ypos;
		sg_FirstMouse = false;
	}

	float xoffset = xpos - sg_lastMouseX;
	float yoffset = ypos - sg_lastMouseY;
	sg_lastMouseX = xpos;
	sg_lastMouseY = ypos;
	camera.cameraMouseMovement(xoffset, -yoffset);
}
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	camera.cameraMouseScroll(yoffset);
}

GLRenderCore::GLRenderCore()
{
	sg_FirstMouse = true;
}

GLRenderCore::~GLRenderCore()
{
	glfwTerminate();
}

void GLRenderCore::Init()
{
	initGLFW();
	initWindow();
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))  // 必须要先创建窗口才能调用glad
		cout << "Failed to initialize GLAD" << endl;
	initResource();
	initScence();
}

void GLRenderCore::Run()
{
	while (!glfwWindowShouldClose(window))
	{
		processInput(window);
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		currentFrameObjectUpdate();
		for (auto& renderPass : coreRenderPassArr)
			renderPass.second->use();

		glfwSwapBuffers(window);
		glfwPollEvents();
	}
}

std::string GLRenderCore::addShaderProgram(std::string shaderName, std::string vertexShaderPath, std::string fragmentShaderPath)
{
	coreShaderArr[shaderName] = make_shared<Shader>(vertexShaderPath, fragmentShaderPath);
	return shaderName;
}

string GLRenderCore::addTexture(string textureName, std::string texturePath, unsigned int GL_COLOR_FORMAT_MACRO)
{
	coreTexture2dArr[textureName] = make_shared<Texture2D>(texturePath, GL_COLOR_FORMAT_MACRO);
	return textureName;
}

std::string GLRenderCore::addVBO(std::string VBOName, float* vertices, unsigned int verticesSize)
{
	unsigned int VBO;
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, verticesSize, vertices, GL_STATIC_DRAW);
	coreVBOArr[VBOName] = VBO;
	return VBOName;
}



void GLRenderCore::enableDepthTest()
{
		glEnable(GL_DEPTH_TEST);
}

void GLRenderCore::enableTexture(std::string textureName)
{
	coreTexture2dArr[textureName]->use();
}

void GLRenderCore::processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	float currentFrameTime = glfwGetTime();
	float deltaTime = currentFrameTime - sg_lastFrameTime;
	sg_lastFrameTime = currentFrameTime;
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera.cameraMovement(Camera::CameraMovement::FORWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera.cameraMovement(Camera::CameraMovement::BACKWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera.cameraMovement(Camera::CameraMovement::LEFT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera.cameraMovement(Camera::CameraMovement::RIGHT, deltaTime);

}

void GLRenderCore::initGLFW()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
}

void GLRenderCore::initWindow()
{
	window = glfwCreateWindow(screenWidth, screenHeight, "LearnOpenGL", nullptr, nullptr);
	if (window == nullptr)
	{
		cout << "Failed to create GLFW window";
		glfwTerminate();
	}
	glfwMakeContextCurrent(window);

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);
}

void GLRenderCore::initScence()
{
	shared_ptr<RenderPass> lightPassPtr = make_shared<RPLight>(*this, "LightShader", "CubeVBO");
	shared_ptr<RenderPass> cubePassPtr = make_shared<RPCube>(*this, "CubeShader", "CubeVBO");
	cubePassPtr->addPassTexture("objectDiffuse");
	cubePassPtr->addPassTexture("objectSpecular");

	coreRenderPassArr["LightPass"] = lightPassPtr;
	coreRenderPassArr["CubePass"] = cubePassPtr;
}

void GLRenderCore::initResource()
{
	initVBO();
	initShader();
	initTexture();
}

void GLRenderCore::initVBO()
{
	addVBO("CubeVBO", vertices, verticesSize);
}

void GLRenderCore::initTexture()
{
	addTexture("objectDiffuse", "./resources/textures/container2.png", GL_RGBA);
	addTexture("objectSpecular", "./resources/textures/container2_specular.png", GL_RGBA);
}

void GLRenderCore::initShader()
{
	addShaderProgram("CubeShader", "./src/GLSL/vShader.vert", "./src/GLSL/fShader.frag");
	addShaderProgram("LightShader", "./src/GLSL/vShader.vert", "./src/GLSL/light.frag");
}

void GLRenderCore::updateMVPMatrix()
{
	mat4 view = camera.getViewMatrix();
	mat4 projection = perspective(radians(camera.getFov()), screenWidth / (float)screenHeight, 0.1f, 100.0f);
	for (auto& renderPass : coreRenderPassArr)
	{
		renderPass.second->setViewMatrix(view);
		renderPass.second->setProjectionMatrix(projection);
	}
}

void GLRenderCore::currentFrameObjectUpdate()
{
	updateMVPMatrix();
	coreRenderPassArr["CubePass"]->getPassShader().setVec3("viewPos", camera.getCameraPosition());
	//glm::vec3 lightColor;
	//lightColor.x = sin(glfwGetTime() * 2.0f);
	//lightColor.y = sin(glfwGetTime() * 0.7f);
	//lightColor.z = sin(glfwGetTime() * 1.3f);
	//coreRenderPassArr[1]->getShader()->setVec3("lightColor", lightColor);
}




