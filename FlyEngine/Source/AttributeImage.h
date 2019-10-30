#ifndef _ATTRIBUTE_IMAGE_H_
#define _ATTRIBUTE_IMAGE_H_

#include "ObjectAttribute.h"

#include <string>

using namespace std; 

class FlyObject; 
class Quad; 
class Texture; 

class AttributeImage : public ObjectAttribute
{
public: 
	AttributeImage(FlyObject* _parentObject);
	~AttributeImage(); 

	void Init(); 
	void Update(); 
	void Draw(); 
	void CleanUp(); 

	bool CreateImage(const char* texturePath); 

	Quad* GetQuad() const;
	Texture* GetTexture() const; 

private:
	Quad*		quadMesh;
	Texture*	imageTexture;

	FlyObject*	parentObject; 
	
	float		imageWidth; 
	float		imageHeight; 
};

#endif 