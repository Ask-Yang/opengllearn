#include "AdvanceRenderer.h"
#include "..\header\AdvanceRenderer.h"

extern float cubeVertices[];
extern unsigned int cubeVerticesSize;
extern float planeVertices[];
extern unsigned int planeVerticesSize;

void AdvanceRenderer::Run()
{
}

void AdvanceRenderer::initResource()
{
	addVBO("CubeVBO", cubeVertices, cubeVerticesSize);
	addVBO("PlaneVBO", planeVertices, planeVerticesSize);
	addShaderProgram("AdvanceShader", "advance.vert", "advance.frag");
	addTexture("Marble", "./resource/textures/marble.jpg");
	addTexture("Metal", "./resource/textures/metal.png");
}

void AdvanceRenderer::initScene()
{

}
