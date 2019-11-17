#ifndef _FLY_OBJECT_H_
#define _FLY_OBJECT_H_

#include <list>

#include "Globals.h"
#include "Tool.h"
#include "Transform.h"
#include "BoundingBox.h"

class ImageTool; 
class FlyObject {
public: 
	FlyObject(std::string objectName); 
	~FlyObject(); 

	void Update(); 
	void Draw(); 
	void CleanUp(); 

	std::string GetName() const;
	void SetName(std::string newName); 

	// Tools -----------
	ImageTool* AddImageTool(const char* imageTexturePath);
	Tool* GetTool(std::string toolName); 
	Tool* GetTool(ToolType toolType); 
	std::list<Tool*> GetToolsList() const; 
	void DeleteTool(std::string toolNameToDelete); 

	// Draw UI ---------
	void DrawImageToolSettings(); 

public:
	Transform* transform;
	Tool* selectedTool; 
	BoundingBox* clickableArea; 
	bool isSelected; 

private: 
	std::list<Tool*> toolsList; 
	std::string name; 
	UID id; 
};

#endif

