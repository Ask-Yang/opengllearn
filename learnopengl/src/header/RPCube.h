#pragma once
#include "RenderPass.h"
class RPCube :
    public RenderPass
{
public:
    RPCube(GLRenderCore& renderCoreIn, std::string shaderName, std::string VBOName);
    void init();
private:
    void initVAO();
    void initShader();
    void setDrawMode();
};

