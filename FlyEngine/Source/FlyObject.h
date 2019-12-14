#ifndef _FLY_OBJECT_H_
#define _FLY_OBJECT_H_

#include <list>

#include "Globals.h"
#include "Action.h"
#include "Transform.h"
#include "ScalarBoundingBox.h"
#include "ImGui/imgui.h"
#include "JSON/parson.h"

enum FlyObjectType
{
	ACTION_OBJECT, 
	INVENTORY_ITEM,
};

class Gizmos; 
class DisplayImageAction; 
class ChangeRoomAction; 
class FlyObject {
public: 
	FlyObject(std::string objectName, std::string description = "", FlyObjectType _flyObjectType = ACTION_OBJECT); 
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
	bool IsInventoryItem(); 

	// Save & Load 
	void SaveObjectData(JSON_Object* jsonObject, int objectIndex);
	void SaveTransform(std::string serializeObjectName, JSON_Object* jsonObject);
	void SerializeClickableArea(std::string serializeObjectName, JSON_Object* jsonObject);

	// Actions ---------
	void DoOnClickActions(); 

	// Tools -----------
	DisplayImageAction* AddDisplayImageAction(const char* imageTexturePath);
	ChangeRoomAction* AddChangeRoomAction();

	void SetSelectedAction(ActionType toolTypeSelected); 
	ActionType GetSelectedActionType(); 
	Action* GetAction(std::string toolName); 
	Action* GetAction(ActionType toolType); 
	std::list<Action*> GetActionsList() const; 
	void DeleteAction(std::string toolNameToDelete);

	// Clickable Area ---
	void CreateClickableArea(float2 percentagePos, float2 percentageSize, bool directPosition = false);
	void DrawClickableArea();
	ScalarBoundingBox* GetClickableArea();
	float2 SetCASizeFromOne(float2 percentagePos, float2 percentageSize, bool directPosition = false);
	
	// Sets & Gets ------
	float2& GetClickableAreaPosOne(); 
	float2& GetClickableAreaSizeOne(); 

	void SetClickableAreaPosOne(float2 newAreaPosOne);
	void SetClickableAreaSizeOne(float2 newAreaSizeOne);

	std::string GetName() const;
	void SetName(std::string newName); 

	UID GetUID() const;
	void SetUID(UID newUID);

	std::string GetDescription() const;
	void SetDescription(std::string newDescription);

	float4 GetClickableAreaColor() const;
	void SetClickableAreaColor(float4 newColor);

	bool& IsInteractable();
	void SetInteractable(bool _isInteractable);

private: 
	// Draw UI ---------
	void DrawDisplayImageSettings(); 

public:
	Transform* transform;
	Action* selectedAction; 
	Gizmos* gizmos; 
	
	bool isSelected; 
	bool clickableAreaActive; 
	FlyObjectType flyObjectType; 

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
	UID uid; 
	bool hasVisuals;
	bool isInteractable; 
};

#endif

