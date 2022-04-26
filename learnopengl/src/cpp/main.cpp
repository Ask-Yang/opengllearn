#include "GLRenderCore.h"
#include "Texture.h"
#include "PassRenderer.h"
#include "ModelRenderer.h"
#include "AdvanceRenderer.h"
#include "ShadowRenderer.h"
#include "Renderer/TAARenderer.h"

int main()
{
	TAARenderer glrcore;
	glrcore.Init();
	glrcore.Run();
    return 0;
}


