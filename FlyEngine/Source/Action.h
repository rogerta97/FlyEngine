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
	bool PassConditionTest();

	bool& IsOccSceneEnter();
	bool& IsOccSceneLeave();
	bool& IsOccBlackboardValue();
	bool& IsOccObjectClicked();

	void SetOccSceneEnter(bool newValue);
	void SetOccSceneLeave(bool newValue);
	void SetOccBlackboardValue(bool newValue);
	void SetOccObjectClicked(bool newValue);

	ActionCondition* AddEmptyCondition(ActionConditionType conditionType);

	// UI Draw -------------------------
	void DrawValueConditionsList();
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
	
	// Conditions -----------------
	string showValueConditionButtonText; 
	bool showValueConditions = false; 
	list<ActionCondition*> actionConditions;
	ConditionEvaluationCriteria evaluationCriteria; 

	// Vars -----------------------
	std::string toolName; 
	std::string toolDescription; 
	bool isSelected; 
	bool isVisual; 
}; 

#endif

