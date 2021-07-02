#include "GLRenderCore.h"
#include "Texture.h"

int main()
{
	GLRenderCore glrcore;
	glrcore.Init();
	glrcore.SetShader("./src/GLSL/vShader.vert", "./src/GLSL/fShader.frag");
	std::shared_ptr<Texture2D> texture1 = glrcore.AddTexture("./resources/textures/awesomeface.png", GL_RGBA);
	//texture1->setTextureWrapMode(Texture2D::WrapMode::Clamp);
	//glrcore.AddTexture("./resources/textures/grass.png");
	glrcore.Run();

    return 0;
}


