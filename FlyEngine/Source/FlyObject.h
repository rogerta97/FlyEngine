#ifndef _FLY_OBJECT_H_
#define _FLY_OBJECT_H_

#include <string>
#include <list>

#include "Globals.h"
#include "ObjectAttribute.h"
#include "Transform.h"

class AttributeImage; 
class FlyObject
{
public: 
	FlyObject(std::string objectName); 
	~FlyObject(); 

	void Update(); 
	void Draw(); 

	std::string GetName() const;

	// Attributes -----------
	AttributeImage* AddAttributeImage(const char* imageTexturePath);

public:
	Transform* transform; 
	bool isSelected; 

private: 
	std::list<ObjectAttribute*> attributeList; 
	std::string name; 
	UID id; 
};

#endif

