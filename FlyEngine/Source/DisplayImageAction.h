#ifndef _DISPLAY_IMAGE_ACTION_H_
#define _DISPLAY_IMAGE_ACTION_H_

#include "Action.h"

using namespace std; 

class FlyObject; 
class Quad; 
class Texture; 

class DisplayImageAction : public Action
{
public: 
	DisplayImageAction(FlyObject* _parentObject);
	~DisplayImageAction(); 

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