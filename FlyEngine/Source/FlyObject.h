#ifndef _FLY_OBJECT_H_
#define _FLY_OBJECT_H_

#include <list>

#include "Globals.h"
#include "Action.h"
#include "Transform.h"
#include "ScalarBoundingBox.h"
#include "ImGui/imgui.h"

class Gizmos; 
class DisplayImageAction; 
class ChangeSceneTool; 
class FlyObject {
public: 
	FlyObject(std::string objectName, std::string description = ""); 
	~FlyObject(); 

	void Update(); 
	void Draw();
	void CleanUp(); 

	// Utils -----------
	float2 GetObjectVisualDimensions();  

	void CalculateCurrentGizmo(); 
	void CalculateAllGizmos(); 
	void FitObjectUtils(); 
	bool IsMouseOver(); 

	bool HasVisuals(); 

	// Tools -----------
	DisplayImageAction* AddDisplayImageAction(const char* imageTexturePath);
	ChangeSceneTool* AddChangeRoomAction();

	Action* GetAction(std::string toolName); 
	Action* GetAction(ToolType toolType); 
	std::list<Action*> GetActionsList() const; 
	void DeleteAction(std::string toolNameToDelete);

	// Clickable Area ---
	void DrawClickableArea();
	ScalarBoundingBox* GetClickableArea();
	void CreateClickableArea(float2 percentagePos, float2 percentageSize, bool directPosition = false);
	float2 SetCASizeFromOne(float2 percentagePos, float2 percentageSize, bool directPosition = false);
	
	// Sets & Gets ------
	float2& GetClickableAreaPosOne(); 
	float2& GetClickableAreaSizeOne(); 

	void SetClickableAreaPosOne(float2 newAreaPosOne);
	void SetClickableAreaSizeOne(float2 newAreaSizeOne);

	std::string GetName() const;
	void SetName(std::string newName); 

	std::string GetDescription() const;
	void SetDescription(std::string newDescription);

	float4 GetClickableAreaColor() const;
	void SetClickableAreaColor(float4 newColor);

private: 
	// Draw UI ---------
	void DrawDisplayImageSettings(); 

public:
	Transform* transform;
	Action* selectedAction; 
	Gizmos* gizmos; 
	
	bool isSelected; 
	bool clickableAreaActive; 

private: 
	// Clickable Area -----
	ScalarBoundingBox* clickableArea; 
	float2 clickableAreaPosPerc; 
	float2 clickableAreaSizePerc; 
	float4 clickableAreaColor; 

	// Fly Object Vars ----
	std::list<Action*> actionsList; 
	std::string name; 
	std::string description;
	UID id; 
	bool hasVisuals;
};

#endif

