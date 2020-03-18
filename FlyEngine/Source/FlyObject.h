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
	OBJECT_SEQUENTIAL,
	UI_HOLDER, 
};

class Gizmos; 
class DisplayImageAction; 
class ModifyVariableAction; 
class DisplayTextAction; 
class ChangeRoomAction; 
class Room; 
class EmitSoundAction; 
class DisplayAnimationAction;
class FollowPathAction; 
class FlyObject {
public: 
	FlyObject(std::string objectName, std::string description = "", FlyObjectType _flyObjectType = ACTION_OBJECT, Room* parentRoom = nullptr); 
	FlyObject();
	~FlyObject(); 

	bool Update(float dt);

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
	void SaveClickableArea(std::string serializeObjectName, JSON_Object* jsonObject);

	// Actions ---------
	virtual void DoObjectActions(); 
	void DoOnClickActions(); 
	void DoOnMouseOverActions();
	void DoVariableConditionActions(FlyVariable* currentVariableValue);

	// Tools -----------
	DisplayImageAction* AddDisplayImageAction(const char* imageTexturePath = "None", bool addToSequentialActions = false);
	ChangeRoomAction* AddChangeRoomAction(bool addToSequentialActions = false);
	ModifyVariableAction* AddModifyVariableAction(bool addToSequentialActions = false);
	EmitSoundAction* AddEmitSoundAction(bool addToSequentialActions = false);
	DisplayTextAction* AddDisplayTextAction(bool addToSequentialActions = false);
	DisplayAnimationAction* AddDisplayAnimationAction(bool addToSequentialActions = false);
	FollowPathAction* AddFollowPathAction(bool addToSequentialActions = false);

	void SetSelectedAction(ActionType toolTypeSelected); 
	ActionType GetSelectedActionType(); 
	Action* GetAction(std::string toolName); 
	Action* GetAction(ActionType toolType); 
	Action* GetSequentialAction(ActionType toolType);
	std::list<Action*> GetActionsList() const; 
	std::list<Action*> GetSequentialActionsList() const;
	void DeleteAction(ActionType toolNameToDelete);

	// Clickable Area ---
	void CreateClickableArea(float2 percentagePos, float2 percentageSize, bool directPosition = false);
	void DrawClickableArea();
	BoundingBox* GetClickableArea();
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

	Room* GetParentRoom() const;
	void SetParentRoom(Room* newParentRoom);

	bool& IsInteractable();
	void SetInteractable(bool _isInteractable);

	DisplayImageAction* GetDisplayImageAction(); 

private: 
	// Draw UI ---------
	void DrawDisplayImageSettings(); 

public:
	Transform* transform;
	Action* selectedAction; 
	Gizmos* gizmos; 
	
	bool isSelected; 
	FlyObjectType flyObjectType; 

	// Clickable Area Public ------
	bool clickableAreaActive = false;
	bool drawClickableArea = false;
	bool isPicked = false; 

private: 

	// Parent Room --------
	Room* parentRoom; 

	// Clickable Area -----
	BoundingBox* clickableArea; 
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

	// Fly Object Fixed Actions
	std::list<Action*> sequentialActionsList;

};

#endif

