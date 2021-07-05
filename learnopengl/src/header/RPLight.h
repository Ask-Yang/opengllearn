#pragma once
#include "RenderPass.h"
class RPLight :
    public RenderPass
{
public:
    RPLight();
    void init();
private:
    void initVAO();
    void initShader();
    void setDrawMode();
};

