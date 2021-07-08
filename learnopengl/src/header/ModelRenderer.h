#pragma once
#include "GLRenderCore.h"
class ModelRenderer :
    public GLRenderCore
{
public:
    void Run() override;
private:
    void initResource() override;
    void initScene() override;
    void initShader() override;

    void currentFrameObjectUpdate();
    std::shared_ptr<Model> model;
    
};

