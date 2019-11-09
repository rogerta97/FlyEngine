#ifndef _FLY_OBJECT_H_
#define _FLY_OBJECT_H_

#include <list>

#include "Globals.h"
#include "Tool.h"
#include "Transform.h"

class ImageTool; 
class FlyObject
{
public: 
	FlyObject(std::string objectName); 
	~FlyObject(); 

	void Update(); 
	void Draw(); 

	std::string GetName() const;

	// Attributes -----------
	ImageTool* AddAttributeImage(const char* imageTexturePath);

public:
	Transform* transform; 
	bool isSelected; 

private: 
	std::list<Tool*> attributeList; 
	std::string name; 
	UID id; 
};

#endif

