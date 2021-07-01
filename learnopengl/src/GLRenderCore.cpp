#include "GLRenderCore.h"

using namespace std;

float vertices[] = { // extern 的数组不能求sizeof
	-0.5f, -0.5f, 0.0f,
	 0.5f, -0.5f, 0.0f,
	 0.0f,  0.5f, 0.0f
};


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

		glUseProgram(shaderProgram);
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 3);

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
	compileVertexShader();
	compileFragmentShader();
	linkShaderProgram();
	activeShaderProgram();
}

void GLRenderCore::buildVAO()
{
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
}



void GLRenderCore::compileVertexShader()
{
	const char* vertexShaderSource = "#version 330 core\n"
		"layout (location = 0) in vec3 aPos;\n"
		"void main()\n"
		"{\n"
		"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
		"}\0";
	
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, nullptr);
	glCompileShader(vertexShader);
	checkErrorInformation(vertexShader, GL_COMPILE_STATUS);
}

void GLRenderCore::compileFragmentShader()
{
	const char* fragmentShaderSource = "#version 330 core\n"
		"out vec4 FragColor;\n"
		"void main()\n"
		"{\n"
		"	FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
		" }\0";
	
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, nullptr);
	glCompileShader(fragmentShader);
	checkErrorInformation(fragmentShader, GL_COMPILE_STATUS);
}

void GLRenderCore::linkShaderProgram()
{
	shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);
	checkErrorInformation(shaderProgram, GL_LINK_STATUS);
}

void GLRenderCore::activeShaderProgram()
{
	glUseProgram(shaderProgram);
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
}

void GLRenderCore::checkErrorInformation(GLuint objectID, GLenum pname)
{
	int success;
	char infoLog[512];
	if (pname == GL_COMPILE_STATUS)
	{
		glGetShaderiv(objectID, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(objectID, 512, nullptr, infoLog);
			cout << "ERROR::SHADER::COMPILATION_FAILED\n" << infoLog << std::endl;
		}
	}
	else if (pname == GL_LINK_STATUS)
	{
		glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
		if (!success) {
			glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
			cout << "ERROR::PROGRAM::LINKED_FAILED\n" << infoLog << std::endl;
		}
	}
	
}


