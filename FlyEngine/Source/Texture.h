#ifndef __TEXTURE_H__
#define __TEXTURE_H__

#include "Globals.h"
#include "OpenGL.h"
#include "Resource.h"
#include "imgui.h"
#include <string>

class Texture : public Resource
{
public:
	Texture();
	~Texture();

	//Utility
	void SetTextureSettings();
	float GetAspectRatio(); 
	bool IsVertical(); 
	ImVec2 GetImageSizeInSquare(ImVec2 squareSize);

	//Memory management
	void Bind();
	void UnBind();

	//Setters & Getters 
	uint GetTextureID() const;
	void SetTextureID(uint id);

	void SetWidth(float width);
	unsigned int GetWidth() const;

	void SetHeigth(float height);
	unsigned int GetHeigth() const;

	//Buffers
	uint CreateBuffer();
	void CleanUp();

	uint texture_id = -1;

private:
	uint width = 0;
	uint height = 0;
};

#endif
