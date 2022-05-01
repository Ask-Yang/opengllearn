#pragma once
#include "PassRenderer.h"
#include <queue>
class TAARenderer :
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

private:
	void RecordMatrix();
	void Render_Velocity();
	void Render_Taa();
	void Render_WithoutJitter();
	void Render_PostProcess();
	void Render_ShadowMap();
	void JitteringProjection();
	int TemporalSampleIndex = 0;
	const float PI = 3.1415926535;

	float Halton[16] = {
		-0.163972363, 0.284008324,
		-0.208000556, -0.360267729,
		0.172162965, 0.144461900,
		-0.430473328, 0.156679258,
		0.0485312343, -0.275233328,
		0.0647613853, 0.367280841,
		-0.147184864, -0.0535709597,
		0.366960347, -0.307915747
	};

	glm::mat4 lastView;
	glm::mat4 lastProjection;
	std::queue<glm::mat4> lastModelMatrixQueue;
	std::map<int, glm::mat4> currentModelMatrixMap;
};