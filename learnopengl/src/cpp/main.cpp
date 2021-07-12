#include "GLRenderCore.h"
#include "Texture.h"
#include "PassRenderer.h"
#include "ModelRenderer.h"
#include "AdvanceRenderer.h"
int main()
{
	AdvanceRenderer glrcore;
	glrcore.Init();
	glrcore.enableDepthTest();
	glrcore.Run();
    return 0;
}


