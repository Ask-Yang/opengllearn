#pragma once
#include "PassRenderer.h"
#include "RPAdvance.h"
class AdvanceRenderer :
    public PassRenderer
{
public:
    void Run() final;
private:
    void initResource() final;
    void initScene() final;
};

