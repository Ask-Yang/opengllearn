#pragma once
#include <string>
#include <iostream>
#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "stb_image.h"
#include "Shader.h"
class Texture2D
{
public:
	enum class WrapMode : unsigned char {
		Default, Repeat, Clamp, Mirror
	};
	enum class FilterMode : unsigned char{
		Default, Point, Bilinear, Trilinear
	};

public:
	Texture2D(std::string texturePath);
	void use();
	void use(int GL_TEXTURE_MACRO);
	unsigned int getTextureID() {
		return texture;
	}
	//void setShader(Shader &shader);
	void setTextureWrapMode(WrapMode wm);
	void setTextureFilterMode(FilterMode fm);
private:
	void bindGLTexture();

	//std::shared_ptr<Shader> textureShader;

	unsigned int texture = 0;
	WrapMode textureWrapMode = WrapMode::Default;
	FilterMode textureFilterMode = FilterMode::Default;
	int width = 0;
	int height = 0;
	int nrChannels = 0;
	unsigned char* data = nullptr;
};

