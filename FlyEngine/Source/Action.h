#ifndef _ACTION_H_
#define _ACTION_H_

#include <string>
#include <list>
#include "JSON/parson.h"
#include "ActionCondition.h"

using namespace std; 

enum ActionType
{
	ACTION_DISPLAY_IMAGE,
	ACTION_CHANGE_ROOM,
	ACTION_MOD_VARIABLE,
	ACTION_EMIT_SOUND,
	ACTION_DISPLAY_TEXT,
	AT_null,
};

enum ConditionEvaluationCriteria
{
	ALL_SUCCED,
	ONE_SUCCED,
};

enum ActionOccurrence
{
	ON_SCENE_ENTER, 
	ON_SCENE_LEAVE,
	ON_OBJECT_CLICKED,
	ON_CONTINUOUS,
	ON_None
};

struct ActionSelectableInfo
{
	std::string actionName;
	std::string actionDescription;
	ActionType actionType;
};

class FlyObject; 
class ActionCondition; 
class ActionConditionHasItem; 
class ActionConditionVariable; 
class Blackboard; 
class Action
{
public:
	Action();
	~Action();

	virtual void Init();
	virtual void Update();
	virtual void Draw();
	virtual void CleanUp();

	virtual void DrawUISettings(); 
	virtual void DrawUISettingsInButton(); 

	// Save & Load --------------------
	virtual void SaveAction(JSON_Object* jsonObject, string serializeObjectString, bool literalStr = false);

	// Occurrence ---------------------
	void SaveOccurrence(JSON_Object* jsonObject, string serializeObjectString);
	void SaveActionConditions(std::string& serializeObjectString, JSON_Object* jsonObject);
	void LoadOccurrence(JSON_Object* jsonObject, string serializeObjectString); 

	// Actions ------------------------
	virtual void DoAction(); 

	// Occurrence & Conditions --------
	void LoadConditions(JSON_Object* jsonObject, string serializeObjectString); 
	ActionConditionHasItem* LoadConditionHasItem(JSON_Object* jsonObject, string serializeObjectString);
	ActionConditionVariable* LoadConditionVariable(JSON_Object* jsonObject, string serializeObjectString, Blackboard* currentBlackboard);

	bool PassConditionTest();

	bool& IsOccSceneEnter();
	bool& IsOccSceneLeave();
	bool& IsOccCondition();
	bool& IsOccMouseOver();
	bool& IsOccObjectClicked();

	void SetOccSceneEnter(bool newValue);
	void SetOccSceneLeave(bool newValue);
	void SetOccBlackboardValue(bool newValue);
	void SetOccObjectClicked(bool newValue);

	ActionCondition* AddEmptyCondition(ActionConditionType conditionType);
	void AddCondition(ActionCondition* newCondition);

	// UI Draw -------------------------
	void DrawActionConditionsList();
	void OnAddConditionButtonPressed();

	// Set and Get ---------------------
	std::string GetActionName() const; 
	void SetActionName(std::string newName);

	FlyObject* GetParentObject() const;
	void SetParentObject(FlyObject* newName);

	std::string GetToolDescription() const;
	void SetToolDescription(std::string newDescription);

	ActionType GetActionType() const;
	void SetToolType(ActionType newToolType);

	bool& IsSelected(); 
	void SetIsSelected(bool _isSelected);

	void SetSelected(bool newSelected); 
	bool& HasVisual(); 

	ActionSelectableInfo GetActionSelectableInfo(); 
 
protected:
	ActionType actionType;
	FlyObject* parentObject;

	// Occurrence -----------------
	bool occ_SceneEnter = false; 
	bool occ_SceneLeave = false;
	bool occ_ObjectClicked = false;
	bool occ_blackboardValue = false;
	bool occ_continuous = false;
	bool occ_mouseOver = false; 
	
	// Conditions -----------------
	string showValueConditionButtonText; 
	bool showConditionsUI = false; 
	list<ActionCondition*> actionConditions;
	ConditionEvaluationCriteria evaluationCriteria; 

	// Vars -----------------------
	std::string toolName; 
	std::string toolDescription; 
	bool isSelected; 
	bool isVisual; 
}; 

#endif

