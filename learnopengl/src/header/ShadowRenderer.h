#pragma once
#include "PassRenderer.h"
class ShadowRenderer :
    public PassRenderer
{
public:
	virtual void Run() override;

private:
	virtual void initScene() override;
	virtual void initResource() override;
	virtual void initVAO() override;
	virtual void initShaderContent() override;
	void depthMapMatrix();
	void renderScene(Shader& shader);
	
};

