#ifndef _ATTRIBUTE_IMAGE_H_
#define _ATTRIBUTE_IMAGE_H_

#include "ObjectAttribute.h"

#include <string>+

using namespace std; 

class FlyObject; 
class Quad; 
class Texture; 

class AttributeImage : public ObjectAttribute
{
public: 
	AttributeImage();
	~AttributeImage(); 

	void Init(); 
	void Update(); 
	void Draw(); 
	void CleanUp(); 

public:

	string		path; 

	float		imageWidth, imageHeight;
	Quad*		quadMesh;
	Texture*	texture;

};

#endif 