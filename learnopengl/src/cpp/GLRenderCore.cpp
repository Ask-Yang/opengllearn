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

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))  // ����Ҫ�ȴ������ڲ��ܵ���glad
	{
		cout << "Failed to initialize GLAD" << endl;
	}

	buildVAO();
}

void GLRenderCore::Run()
{
	while (!glfwWindowShouldClose(window))
	{
		glClearColor(0.2f, 0.4f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		coreShader->use();
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 3);
		//glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);

		processInput(window);
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
}

void GLRenderCore::SetShader(std::string vertexShaderPath, std::string fragmentShaderPath)
{
	coreShader = make_shared<Shader>(vertexShaderPath, fragmentShaderPath);
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

	// λ������
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// ��ɫ����
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
}





