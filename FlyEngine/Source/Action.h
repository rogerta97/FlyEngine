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
	ACTION_DISPLAY_ANIMATION,
	ACTION_FOLLOW_PATH,
	ACTION_DIALOG,
	AT_null,
};

enum ActionClass
{
	ACTION_CLASS_DIRECT,
	ACTION_CLASS_SEQUENTIAL,
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
	virtual void Update(float dt);
	virtual void Draw();
	virtual void CleanUp();

	virtual void DrawUISettings(); 
	virtual void DrawUISettingsInButton(); 

	// Utility ------------------------
	bool IsActionSequential(); 

	// Save & Load --------------------
	virtual void SaveAction(JSON_Object* jsonObject, string serializeObjectString, bool literalStr = false, int actionPositionInObject = 0);

	// Occurrence ---------------------
	void SaveOccurrence(JSON_Object* jsonObject, string serializeObjectString);
	void SaveActionConditions(std::string& serializeObjectString, JSON_Object* jsonObject);
	void LoadOccurrence(JSON_Object* jsonObject, string serializeObjectString); 

	// Actions ------------------------
	virtual void DoAction(); 
	virtual void StopAction(); 

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

	ActionType GetType() const;
	void SetToolType(ActionType newToolType);

	bool& IsSelected(); 
	void SetIsSelected(bool _isSelected);

	bool& IsInfoHolder();
	void SetIsInfoHolder(bool _isInfoHolder);

	bool& GetIsSequencial();
	void SetIsSequencial(bool _isSequential);

	void SetSelected(bool newSelected); 
	bool& HasVisual(); 

	void SetDrawIfSequential(bool _dis);
	bool& GetDrawIfSequential();

	virtual void SetVisible(bool isVisible);
	bool& GetIsVisible();

	UID GetUID();
	void SetUID(UID newUID);

	ActionClass GetActionClass();
	void SetActionClass(ActionClass _acSec);

	ActionSelectableInfo GetActionSelectableInfo(); 

	bool IsActionFinished(); 
	void SetActionCompleted(bool isFinished);
 
protected:
	ActionType actionType;
	FlyObject* parentObject;
	bool actionFinished; 
	ActionClass actionClass; 
	UID uid; 

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
	
	bool isSelected = false; 
	bool isVisual = false; 
	bool isSequential = false; 
	bool isDataAttached = false; 
	bool isHolderInfo = false;
	bool drawIfSequential = true; 
	bool isVisible = true; 
}; 

#endif

