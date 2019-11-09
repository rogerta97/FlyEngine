#ifndef __TEXTURE_H__
#define __TEXTURE_H__

#include "Globals.h"
#include "OpenGL.h"
#include "Resource.h"
#include <string>

class Texture : public Resource
{
public:
	Texture();
	~Texture();

	//Utility
	void SetTextureSettings();

	//Memory management
	void Bind();
	void UnBind();

	//Setters & Getters 
	uint GetTextureID() const;
	void SetTextureID(uint id);

	void SetWidth(float width);
	unsigned int GetWidth() const;

	void SetHeight(float height);
	unsigned int GetHeight() const;

	//Buffers
	uint CreateBuffer();
	void Clear();

	uint texture_id = -1;

private:
	uint width = 0;
	uint height = 0;



};

#endif#
