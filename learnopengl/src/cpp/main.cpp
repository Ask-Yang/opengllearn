#include "GLRenderCore.h"

int main()
{
	GLRenderCore glrcore;
	glrcore.Init();
	glrcore.SetShader("./src/GLSL/vShader.vert", "./src/GLSL/fShader.frag");
	glrcore.AddTexture("./resources/textures/awesomeface.png");
	glrcore.AddTexture("./resources/textures/container.jpg");

	glrcore.Run();
}