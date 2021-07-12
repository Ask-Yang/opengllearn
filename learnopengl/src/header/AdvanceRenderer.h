#pragma once
#include "PassRenderer.h"
#include "RPAdvance.h"
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
   
};

