#pragma once
#include "RenderPass.h"
class RPPostProcess :
    public RenderPass
{
public:
    RPPostProcess(PassRenderer& renderCoreIn, std::string shaderName, std::string VBOName);
    void init() override;
private:
    void initVAO() override;
    void initShader() override;
    void setDrawMode() override;
};

