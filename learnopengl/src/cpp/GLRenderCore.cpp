#include "GLRenderCore.h"

using namespace std;
using namespace glm;

extern float vertices[];
extern unsigned int verticesSize;
extern unsigned int indices[];
extern unsigned int indicesSize;
extern vec3 cubePositions[];

static Camera camera(vec3(0, 0, 3), vec3(0, 0, 0), vec3(0, 1, 0)); // 那如果有多个窗口如何实现呢？
// 可能的视线：每个类保存自己的这个对应的变量，然后要用的时候就取这个全局变量的值，进行计算，看情况进行覆写
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
	{
		cout << "Failed to initialize GLAD" << endl;
	}

	buildVAO();
}

void GLRenderCore::Run()
{
	while (!glfwWindowShouldClose(window))
	{
		processInput(window);
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		for(int i=0;i<coreTexture2dArr.size();i++)
		{
			coreTexture2dArr[i]->use(GL_TEXTURE0 + i);
		}
		if(coreShader)
			coreShader->use();

		mat4 model = mat4(1.0f);
		mat4 view = camera.getViewMatrix();
		mat4 projection = perspective(radians(camera.getFov()), screenWidth / (float)screenHeight, 0.1f, 100.0f);
		//coreShader->setMat4("model", model);
		coreShader->setMat4("view", view);
		coreShader->setMat4("projection", projection);

		glBindVertexArray(VAO);
		
		for (unsigned int i = 0; i < 10; i++)
		{
			model = translate(model, cubePositions[i]);
			float angle = 20.0f * i;
			model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
			coreShader->setMat4("model", model);

			glDrawArrays(GL_TRIANGLES, 0, 36);
		}
		// glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		
	
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
}

void GLRenderCore::SetShader(std::string vertexShaderPath, std::string fragmentShaderPath)
{
	coreShader = make_shared<Shader>(vertexShaderPath, fragmentShaderPath);
	coreShader->use();

	coreShader->setTexture("texture1", 0);
	coreShader->setTexture("texture2", 1);
}

shared_ptr<Texture2D> GLRenderCore::AddTexture(std::string texturePath, unsigned int GL_COLOR_FORMAT_MACRO)
{
	coreTexture2dArr.push_back(make_shared<Texture2D>(texturePath, GL_COLOR_FORMAT_MACRO));
	return coreTexture2dArr.back();
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


void GLRenderCore::buildVAO()
{
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	//glGenBuffers(1, &EBO);
	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, verticesSize, vertices, GL_STATIC_DRAW);

	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	//glBufferData(GL_ELEMENT_ARRAY_BUFFER, indicesSize, indices, GL_STATIC_DRAW);

	// 位置属性
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	//// 颜色属性
	//glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	//glEnableVertexAttribArray(1);

	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
}


