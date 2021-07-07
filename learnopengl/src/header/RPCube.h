#pragma once
#include "RenderPass.h"

class RPCube :
    public RenderPass
{
public:
    RPCube(GLRenderCore& renderCoreIn, std::string shaderName, std::string VBOName);
    void init() override;
private:
    void initVAO() override;
    void initShader() override;
    void setDrawMode () override;
};

