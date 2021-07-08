#include "GLRenderCore.h"
#include "Texture.h"
#include "PassRenderer.h"
#include "ModelRenderer.h"
int main()
{
	ModelRenderer glrcore;
	glrcore.Init();
	glrcore.enableDepthTest();
	glrcore.Run();
    return 0;
}


