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
	OBJECT_CHARACTER, 
	UI_HOLDER, 
};

enum SequentialConditionEvaluationCriteria
{
	SEQ_ALL_SUCCED, 
	SEQ_ONE_SUCCED
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
class DialogueAction;
class FlyObject {
public: 
	FlyObject(std::string objectName, std::string description = "", FlyObjectType _flyObjectType = ACTION_OBJECT, Room* parentRoom = nullptr); 
	FlyObject();
	~FlyObject(); 

	virtual bool Update(float dt);
	virtual void OnSceneEnter(); 
	virtual void OnSceneLeave(); 

	void DrawVisualLayer(bool forceDraw = false);
	void DrawOverlaysLayer(); 

	void CleanUp(); 

	// Utils -----------
	float2 GetObjectVisualDimensions();  
	void CalculateCurrentGizmo(); 
	void CalculateAllGizmos(); 
	void FitObjectUtils(); 
	bool IsMouseOver(); 

	void CopyFlyObjectActions(FlyObject* objectToClone);

	bool HasVisuals(); 
	bool IsInventoryItem(); 

	// Save & Load 
	virtual void SaveObjectData(JSON_Object* jsonObject, int objectIndex);
	void SaveTransform(std::string serializeObjectName, JSON_Object* jsonObject);
	void SaveClickableArea(std::string serializeObjectName, JSON_Object* jsonObject);

	// Actions ---------
	virtual void DoObjectActions(); 
	void DoOnClickActions(); 
	void DoOnMouseOverActions();
	void DoVariableConditionActions(FlyVariable* currentVariableValue);
	bool HasAction(ActionType _actionToCheckType);
	bool HasSequentialAction(ActionType _actionToCheckType);

	// Tools -----------
	DisplayImageAction* AddDisplayImageAction(const char* imageTexturePath = "None", bool addToSequentialActions = false);
	ChangeRoomAction* AddChangeRoomAction(bool addToSequentialActions = false);
	ModifyVariableAction* AddModifyVariableAction(bool addToSequentialActions = false);
	EmitSoundAction* AddEmitSoundAction(bool addToSequentialActions = false);
	DisplayTextAction* AddDisplayTextAction(bool addToSequentialActions = false);
	DisplayAnimationAction* AddDisplayAnimationAction(bool addToSequentialActions = false);
	FollowPathAction* AddFollowPathAction(bool addToSequentialActions = false);
	DialogueAction* AddDialogueAction(bool addToSequentialActions = false);

	void DrawSequentialActionsList();
	void SetSelectedAction(UID toolTypeSelected, bool isSequential = false);

	ActionType GetSelectedActionType(); 
	Action* GetAction(std::string toolName); 
	Action* GetAction(ActionType toolType); 
	Action* GetAction(UID actionUID); 
	Action* GetSequentialAction(ActionType toolType);
	std::list<Action*> GetActionsList() const; 
	std::list<Action*> GetSequentialActionsList() const;
	void DeleteAction(ActionType toolNameToDelete);
	void DeleteSequentialAction(UID toolNameToDelete);
	void DeleteAction(UID toolNameToDelete);

	void MoveSequentialAction(UID actionUID, int moveType);

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

	Action* GetSelectedAction(); 

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

	SequentialConditionEvaluationCriteria sequentialEvaluationCriteria;
	std::list<ActionCondition*> sequentialActionConditions;
	bool occ_SceneEnter = false;
	bool occ_ObjectClicked = false;
	bool occ_blackboardValue = false;
	bool occ_mouseOver = false;

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

protected: 

	// Sequential Actions Play Control ----
	Action* currentSequentialAction; 

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
	bool sequentialSwapedTick; 

	// Fly Object Fixed Actions
	std::list<Action*> sequentialActionsList;
};

#endif

