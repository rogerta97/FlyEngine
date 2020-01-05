#include "Texture.h"
#include "OpenGL.h"
#include "mmgr.h"

Texture::Texture() : Resource(RESOURCE_TEXTURE)
{
	texture_id = 0;
	height = width = 0; 
}

Texture::~Texture()
{
}

void Texture::SetWidth(float width)
{
	this->width = width;
}

unsigned int Texture::GetWidth() const
{
	return width;
}

void Texture::SetHeigth(float height)
{
	this->height = height;
}

unsigned int Texture::GetHeigth() const
{
	return height;
}

void Texture::SetTextureSettings()
{
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
}

float Texture::GetAspectRatio()
{
	return (float)width / (float)height;
}

bool Texture::IsVertical()
{
	if(height > width)
		return true;

	return false; 
}

void Texture::Bind()
{
	glBindTexture(GL_TEXTURE_2D, texture_id);
}

void Texture::UnBind()
{
	glBindTexture(GL_TEXTURE_2D, 0);
}

uint Texture::GetTextureID() const
{
	return texture_id;
}

void Texture::SetTextureID(uint id)
{
	texture_id = id;
}

uint Texture::CreateBuffer()
{
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glGenTextures(1, &texture_id);

	return texture_id;
}

void Texture::CleanUp()
{
	glDeleteTextures(1, &texture_id);
	texture_id = 0;
}


