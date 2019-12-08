#ifndef _ACTION_H_
#define _ACTION_H_

#include <string>
#include "JSON/parson.h"

using namespace std; 

enum ToolType
{
	AT_IMAGE,
	AT_CHANGE_ROOM,
	AT_null,
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
	ToolType actionType;
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

	virtual void SaveAction(JSON_Object* jsonObject, string serializeObjectString);

	virtual void DoAction(); 

	// Set and Get ---------------------
	std::string GetActionName() const; 
	void SetActionName(std::string newName);

	bool& IsOccSceneEnter();
	bool& IsOccSceneLeave();
	bool& IsOccObjectClicked();

	void SetOccSceneEnter(bool newOccSceneEnter);
	void SetOccSceneLeave(bool newOccSceneLeave);
	void SetOccObjectClicked(bool newOccObjectClicked);

	FlyObject* GetParentObject() const;
	void SetParentObject(FlyObject* newName);

	std::string GetToolDescription() const;
	void SetToolDescription(std::string newDescription);

	ToolType GetActionType() const;
	void SetToolType(ToolType newToolType);

	bool& IsSelected(); 
	bool& HasVisual(); 

	ActionSelectableInfo GetActionSelectableInfo(); 
 
protected:
	ToolType toolType;
	FlyObject* parentObject;

	// Occurrence ----------------
	bool occ_SceneEnter = false; 
	bool occ_SceneLeave = false;
	bool occ_ObjectClicked = false;
	bool occ_continuous = false;

	std::string toolName; 
	std::string toolDescription; 
	bool isSelected; 
	bool isVisual; 
}; 

#endif

