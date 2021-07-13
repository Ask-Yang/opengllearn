#pragma once
#include "PassRenderer.h"
#include "RPAdvance.h"
#include "RPPostProcess.h"
#include "RPSkybox.h"
#include <map>
class AdvanceRenderer :
    public PassRenderer
{
public:
    void Run() final;
private:
    void initResource() final;
    void initScene() final;

    void StencilRun();
    void TransparencyRun();
    void FrameBufferRun();
    void SkyboxRun();
private:
    void loadCubemap();
    void customizeFrameBuffer();
    void drawScene();
private:
    unsigned int cubemap;
    unsigned int texColorBuffer;
    unsigned int framebuffer;
};

