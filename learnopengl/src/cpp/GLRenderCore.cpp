#include "GLRenderCore.h"

using namespace std;
using namespace glm;



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
	pCamera = &camera;
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
	initScene();
}


std::string GLRenderCore::addShaderProgram(std::string shaderName, std::string vertexShaderPath, std::string fragmentShaderPath)
{
	coreShaderArr[shaderName] = make_shared<Shader>(vertexShaderPath, fragmentShaderPath);
	return shaderName;
}

void GLRenderCore::frameUpdateViewProjectionMatrix()
{
	mat4 view = pCamera->getViewMatrix();
	mat4 projection = perspective(radians(pCamera->getFov()), screenWidth / (float)screenHeight, 0.1f, 100.0f);
	for (auto& pShader : coreShaderArr)
	{
		pShader.second->setMat4("view", view);
		pShader.second->setMat4("projection", projection);
	}
		
}


void GLRenderCore::enableDepthTest()
{
	glEnable(GL_DEPTH_TEST);
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




