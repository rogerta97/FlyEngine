#ifndef _ACTION_H_
#define _ACTION_H_

#include <string>
#include <list>
#include "JSON/parson.h"

using namespace std; 

enum ActionType
{
	ACTION_DISPLAY_IMAGE,
	ACTION_CHANGE_ROOM,
	ACTION_MOD_VARIABLE,
	ACTION_EMIT_SOUND,
	AT_null,
};

enum EvaluationCriteria
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

enum ActionConditionOperator
{
	AC_EQUALS_TO, 
	AC_BIGGER_THAN,
	AC_LESS_THAN, 
	AC_DIFFERENT_THAN,
	AC_None
};

struct ActionSelectableInfo
{
	std::string actionName;
	std::string actionDescription;
	ActionType actionType;
};

class FlyVariable; 
class ActionCondition
{
public: 
	ActionCondition();
	~ActionCondition(); 

	void CleanUp(); 
	void SaveCondition(JSON_Object* jsonObject, string serializeObjectString, int pos);

	FlyVariable* targetVariable; 
	ActionConditionOperator actionConditionOperator;

	int targetValueInteger; 
	bool targetValueBoolean; 
};

class FlyObject; 
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
	void LoadOccurrence(JSON_Object* jsonObject, string serializeObjectString); 

	// Actions ------------------------
	virtual void DoAction(); 

	// Occurrence & Conditions --------
	bool& IsOccSceneEnter();
	bool& IsOccSceneLeave();
	bool& IsOccBlackboardValue();
	bool& IsOccObjectClicked();

	void SetOccSceneEnter(bool newValue);
	void SetOccSceneLeave(bool newValue);
	void SetOccBlackboardValue(bool newValue);
	void SetOccObjectClicked(bool newValue);

	ActionCondition* AddEmptyCondition();

	// UI Draw -------------------------
	void DrawValueConditionsList(); 

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
	list<ActionCondition*> actionVariableConditions;
	EvaluationCriteria evaluationCriteria; 

	// Vars -----------------------
	std::string toolName; 
	std::string toolDescription; 
	bool isSelected; 
	bool isVisual; 
}; 

#endif

