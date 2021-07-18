#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "stb_image.h"
#include "Shader.h"
#include "Texture.h"
#include <iostream>
extern float planeVertices[];
extern unsigned int planeVerticesSize;
extern float cubeVertices[];
extern unsigned int cubeVerticesSize;


unsigned int runTime = 10000;

void testGLBindBuffer() 
{
	static unsigned int VBO1;
	static unsigned int VBO2;
	static unsigned int VBO3;
	glGenBuffers(1, &VBO1);
	glGenBuffers(1, &VBO2);
	glGenBuffers(1, &VBO3);
	glBindBuffer(GL_VERTEX_ARRAY, VBO1);
	glBufferData(GL_ARRAY_BUFFER, cubeVerticesSize, cubeVertices, GL_STATIC_DRAW);
	glBindBuffer(GL_VERTEX_ARRAY, VBO2);
	glBufferData(GL_ARRAY_BUFFER, cubeVerticesSize, cubeVertices, GL_STATIC_DRAW);
	glBindBuffer(GL_VERTEX_ARRAY, VBO3);
	glBufferData(GL_ARRAY_BUFFER, cubeVerticesSize, cubeVertices, GL_STATIC_DRAW);
	double start = glfwGetTime();
	for (int i = 0; i < runTime; i++)
	{
		glBindBuffer(GL_VERTEX_ARRAY, VBO1);
		glBindBuffer(GL_VERTEX_ARRAY, VBO2);
		glBindBuffer(GL_VERTEX_ARRAY, VBO3);
	}
	std::cout << glfwGetTime() - start << std::endl;
	//0.0001882
	//0.0001602
}

void testGLBindTexture()
{
	Texture2D texture1("./resources/textures/metal.png");
	Texture2D texture2("./resources/textures/metal.png");
	Texture2D texture3("./resources/textures/metal.png");
	double start = glfwGetTime();
	for (int i = 0; i < runTime; i++)
	{
		texture1.use();
		texture2.use();
		texture3.use();
	}
	std::cout << glfwGetTime() - start << std::endl;
	// 0.0002859
}

void testGLUseProgram() 
{
	Shader shader1("./src/GLSL/model.vert", "./src/GLSL/model.frag");
	Shader shader2("./src/GLSL/model.vert", "./src/GLSL/model.frag");
	Shader shader3("./src/GLSL/model.vert", "./src/GLSL/model.frag");
	double start = glfwGetTime();
	for (int i = 0; i < runTime; i++)
	{
		shader1.use();
		shader2.use();
		shader3.use();
	}
	std::cout << glfwGetTime() - start << std::endl;
	// 0.0001929
}

void testGLUniformBuffer()
{
	unsigned int uniformBuffer1;
	unsigned int uniformBuffer2;
	unsigned int uniformBuffer3;
	glGenBuffers(1, &uniformBuffer1);
	glGenBuffers(1, &uniformBuffer2);
	glGenBuffers(1, &uniformBuffer3);
	double start = glfwGetTime();
	for (int i = 0; i < runTime; i++)
	{
		glBindBuffer(GL_UNIFORM_BUFFER, uniformBuffer1);
		glBindBuffer(GL_UNIFORM_BUFFER, uniformBuffer2);
		glBindBuffer(GL_UNIFORM_BUFFER, uniformBuffer3);
	}
	std::cout << glfwGetTime() - start << std::endl;
}

void testGLFrameBuffer()
{
	unsigned int frameBuffer1;
	unsigned int frameBuffer2;
	unsigned int frameBuffer3;
	glGenFramebuffers(1, &frameBuffer1);
	glGenFramebuffers(1, &frameBuffer2);
	glGenFramebuffers(1, &frameBuffer3);
	double start = glfwGetTime();
	for (int i = 0; i < runTime; i++)
	{
		glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer1);
		glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer2);
		glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer3);
	}
	std::cout << glfwGetTime() - start << std::endl;
}

int main156()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

	// glfw window creation
	// --------------------
	unsigned int SCR_WIDTH = 800, SCR_HEIGHT = 600;
	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}
	testGLBindBuffer();
	testGLBindTexture();
	testGLUseProgram();
	testGLUniformBuffer();
	testGLFrameBuffer();
	// texture > vertexArraybuffer = useprogram > uniformBuffer >> framebuffer 而且都不受数据本身大小影响
	// 差不多是2-1.5倍的时间
	return 0;
}