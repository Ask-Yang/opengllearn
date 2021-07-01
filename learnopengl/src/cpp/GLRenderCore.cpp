#include "GLRenderCore.h"

using namespace std;

extern float vertices[];
extern unsigned int verticesSize;
extern unsigned int indices[];
extern unsigned int indicesSize;


void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

GLRenderCore::GLRenderCore()
{

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

	initShader();
	buildVAO();
}

void GLRenderCore::Run()
{
	while (!glfwWindowShouldClose(window))
	{
		glClearColor(0.2f, 0.4f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		glUseProgram(shaderProgram);
		glBindVertexArray(VAO);
		//glDrawArrays(GL_LINE_STRIP, 0, 4);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);

		processInput(window);
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
}

void GLRenderCore::processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
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
	window = glfwCreateWindow(800, 600, "LearnOpenGL", nullptr, nullptr);
	if (window == nullptr)
	{
		cout << "Failed to create GLFW window";
		glfwTerminate();
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
}

void GLRenderCore::initShader()
{
	
}

void GLRenderCore::buildVAO()
{
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);
	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, verticesSize, vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indicesSize, indices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
}





