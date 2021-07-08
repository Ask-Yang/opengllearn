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
	std::string path;
	std::string type;
	Texture2D(std::string texturePath);
	void use();
	void use(int GL_TEXTURE_MACRO);
	void setTextureWrapMode(WrapMode wm);
	void setTextureFilterMode(FilterMode fm);
	unsigned int getTextureSlot() {
		return current_gl_texture_macro - GL_TEXTURE0;
	}
	unsigned int getTextureID() {
		return texture;
	}
private:
	void bindGLTexture(unsigned int GL_COLOR_FORMAT_MACRO);
private:
	static unsigned int gl_texture_macro_index;
	unsigned int current_gl_texture_macro = 0;
	unsigned int texture = 0;
	WrapMode textureWrapMode = WrapMode::Default;
	FilterMode textureFilterMode = FilterMode::Default;
	int width = 0;
	int height = 0;
	int nrChannels = 0;
	unsigned char* data = nullptr;
};

