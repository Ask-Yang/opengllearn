#include "Texture.h"

unsigned int Texture2D::gl_texture_macro_index = GL_TEXTURE0;

Texture2D::Texture2D(std::string texturePath)
{
	current_gl_texture_macro = gl_texture_macro_index++;
	stbi_set_flip_vertically_on_load(true);
	data = stbi_load(texturePath.c_str(), &width, &height, &nrChannels, 0);
	if (!data)
	{
		std::cout << "Failed to load texture" << std::endl;
		return;
	}
	unsigned int format;
	if (nrChannels == 1)
		format = GL_RED;
	else if (nrChannels == 3)
		format = GL_RGB;
	else if (nrChannels == 4)
		format = GL_RGBA;
	bindGLTexture(format);
	setTextureWrapMode(textureWrapMode);
	setTextureFilterMode(textureFilterMode);
	stbi_image_free(data);
}

void Texture2D::use()
{
	glActiveTexture(current_gl_texture_macro);
	glBindTexture(GL_TEXTURE_2D, texture);
}

void Texture2D::use(int GL_TEXTURE_MACRO)
{
	glActiveTexture(GL_TEXTURE_MACRO);
	glBindTexture(GL_TEXTURE_2D, texture);
}


void Texture2D::setTextureWrapMode(WrapMode wm)
{
	glBindTexture(GL_TEXTURE_2D, texture);
	textureWrapMode = wm;
	if (textureWrapMode == WrapMode::Repeat)
	{
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	}
	else if (textureWrapMode == WrapMode::Clamp || textureWrapMode == WrapMode::Default)
	{
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	}
	else if (textureWrapMode == WrapMode::Mirror)
	{
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
	}
}

void Texture2D::setTextureFilterMode(FilterMode fm)
{
	textureFilterMode = fm;
	if (textureFilterMode == FilterMode::Point || textureFilterMode == FilterMode::Default)
	{
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	}
	else if (textureFilterMode == FilterMode::Bilinear)
	{
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	}
	else if (textureFilterMode == FilterMode::Trilinear)
	{
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	}
}

void Texture2D::bindGLTexture(unsigned int GL_COLOR_FORMAT_MACRO)
{
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_COLOR_FORMAT_MACRO, GL_UNSIGNED_BYTE, data);
	glGenerateMipmap(GL_TEXTURE_2D);
}
