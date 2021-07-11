#include "RPAdvance.h"
#include "..\header\RPAdvance.h"


RPAdvance::RPAdvance(PassRenderer& renderCoreIn, std::string shaderName, std::string VBOName)
	:RenderPass(renderCoreIn, shaderName, VBOName)
{
	init();
}

void RPAdvance::init()
{
	initVAO();
	initShader();
}

void RPAdvance::initVAO()
{
	
}

void RPAdvance::initShader()
{
}

void RPAdvance::setDrawMode()
{
}
