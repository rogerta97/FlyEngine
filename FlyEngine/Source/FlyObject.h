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
	void CleanUp(); 

	std::string GetName() const;

	// Tools -----------
	ImageTool* AddImageTool(const char* imageTexturePath);
	std::list<Tool*> GetToolsList() const; 

public:
	Transform* transform; 
	bool isSelected; 

private: 
	std::list<Tool*> toolsList; 
	std::string name; 
	UID id; 
};

#endif

