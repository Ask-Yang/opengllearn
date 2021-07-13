#pragma once
#include "PassRenderer.h"
#include "RPAdvance.h"
#include "RPPostProcess.h"
#include <map>
class AdvanceRenderer :
    public PassRenderer
{
public:
    void Run() final;
private:
    void initResource() final;
    void initScene() final;
    void customizeFrameBuffer();

    void drawScene();
    void StencilRun();
    void TransparencyRun();
    void FrameBufferRun();
   
private:

    unsigned int texColorBuffer;
    unsigned int framebuffer;
};

