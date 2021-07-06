#include "GLRenderCore.h"
#include "Texture.h"

int main()
{
	GLRenderCore glrcore;
	glrcore.Init();
	glrcore.enableDepthTest();
	glrcore.Run();

    return 0;
}


