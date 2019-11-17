#ifndef _IMAGE_TOOL_H_
#define _IMAGE_TOOL_H_

#include "Tool.h"

using namespace std; 

class FlyObject; 
class Quad; 
class Texture; 

class ImageTool : public Tool
{
public: 
	ImageTool(FlyObject* _parentObject);
	~ImageTool(); 

	void Init(); 
	void Update(); 
	void Draw(); 
	void CleanUp(); 

	bool CreateImage(const char* texturePath); 

	Quad* GetQuad() const;
	void SetQuad(Quad* newQuad);
	
	void SetTexture(Texture* newTexture);
	Texture* GetTexture() const; 

private:
	Quad*		quadMesh;
	Texture*	imageTexture;

	FlyObject*	parentObject; 
	
	float		imageWidth; 
	float		imageHeight; 
	bool		isFake; 
};

#endif 