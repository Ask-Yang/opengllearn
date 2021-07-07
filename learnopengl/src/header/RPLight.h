#pragma once
#include "RenderPass.h"
class RPLight :
    public RenderPass
{
public:
    RPLight(GLRenderCore& renderCoreIn, std::string shaderName, std::string VBOName);
    void init() override; 
private:
    void initVAO() override;
    virtual void initShader() override;
    void setDrawMode() override;
};

