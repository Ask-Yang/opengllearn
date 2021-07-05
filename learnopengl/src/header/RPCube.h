#pragma once
#include "RenderPass.h"
class RPCube :
    public RenderPass
{
public:
    RPCube();
    void init();
private:
    void initVAO();
    void initShader();
    void setDrawMode();
};

