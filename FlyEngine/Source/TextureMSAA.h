#ifndef _TEXTUREMSAA_H_
#define _TEXTUREMSAA_H_

#include "Globals.h"

class TextureMSAA
{
public:
	TextureMSAA();
	~TextureMSAA();

	bool Create(uint width, uint height, int msa_lvl);

	void Bind();
	void Render();
	void Unbind();

	void SetTextureID();
	unsigned int GetTextureID() const;

	void SetWidth(uint width);
	unsigned int GetWidth() const;

	void SetHeight(uint height);
	unsigned int GetHeight() const;

	void LoadToMemory();
	void UnloadFromMemory();
	void CleanUp();

private:

	uint fbo_id;
	uint fbo_msaa_id;
	uint texture_id;
	uint rbo_id;
	uint rbo_color_id;
	uint rbo_depth_id;

	unsigned int width;
	unsigned int height;

	int max_msaa_samples;
	int current_msaa_samples;
};

#endif

