#pragma once
#include "RPLight.h"
class RPPointLight :
    public RPLight
{
public:
    RPPointLight(GLRenderCore& renderCoreIn, std::string shaderName, std::string VBOName);
private:
    void initShader() override;
};

