#include "GLRenderCore.h"
#include "Texture.h"
#include "PassRenderer.h"
#include "ModelRenderer.h"
#include "AdvanceRenderer.h"
#include "ShadowRenderer.h"
int main()
{
	ShadowRenderer glrcore;
	glrcore.Init();
	glrcore.Run();
    return 0;
}


