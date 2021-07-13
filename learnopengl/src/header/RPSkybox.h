#pragma once
#include "RenderPass.h"
class RPSkybox :
    public RenderPass
{
public:
    RPSkybox(PassRenderer& renderCoreIn, std::string shaderName, std::string VBOName);
    void init() override;
private:
    void initVAO() override;
    void initShader() override;
    void setDrawMode() override;
};

