#ifndef _FLY_OBJECT_H_
#define _FLY_OBJECT_H_

#include <list>

#include "Globals.h"
#include "Tool.h"
#include "Transform.h"
#include "BoundingBox.h"

class Gizmos; 
class ImageTool; 
class FlyObject {
public: 
	FlyObject(std::string objectName); 
	~FlyObject(); 

	void Update(); 
	void Draw();
	void DrawClickableArea();

	void CleanUp(); 

	std::string GetName() const;
	void SetName(std::string newName); 

	void Move();
	void Rotate(); 
	void Scale(); 

	// Utils -----------
	float2 GetObjectVisualDimensions();  

	void CalculateCurrentGizmo(); 
	void CalculateAllGizmos(); 
	bool IsMouseOver(); 

	bool HasVisuals(); 

	// Tools -----------
	ImageTool* AddImageTool(const char* imageTexturePath);
	Tool* GetTool(std::string toolName); 
	Tool* GetTool(ToolType toolType); 
	std::list<Tool*> GetToolsList() const; 
	void DeleteTool(std::string toolNameToDelete);

	// Clickable Area ---
	BoundingBox* GetClickableArea();
	void SetCASizeFromOne(float2 percentagePos, float2 percentageSize, bool directPosition = false);
	
	// Sets & Gets ------
	float2& GetClickableAreaPosOne(); 
	float2& GetClickableAreaSizeOne(); 

	void SetClickableAreaPosOne(float2 newAreaPosOne);
	void SetClickableAreaSizeOne(float2 newAreaSizeOne);

private: 
	// Draw UI ---------
	void DrawImageToolSettings(); 

public:
	Transform* transform;
	Tool* selectedTool; 
	Gizmos* gizmos; 
	
	bool isSelected; 
	bool clickableAreaActive; 

private: 
	BoundingBox* clickableArea; 
	float2 clickableAreaPosOne; 
	float2 clickableAreaSizeOne; 

	std::list<Tool*> toolsList; 
	std::string name; 
	UID id; 
	bool hasVisuals;
};

#endif

