#ifndef _ACTION_H_
#define _ACTION_H_

#include <string>

enum ToolType
{
	AT_IMAGE,
	AT_CHANGE_SCENE,
	AT_null,
};

enum ActionOccurrence
{
	occ_SceneEnter, 
	occ_SceneLeave,
	occ_AreaClicked,
	occ_continuous,
	occ_None
};

struct ActionSelectableInfo
{
	std::string actionName;
	std::string actionDescription;
	ToolType actionType;
};

class Action
{
public:
	Action();
	~Action();

	virtual void Init();
	virtual void Update();
	virtual void Draw();
	virtual void CleanUp();

	// Set and Get ---------------------
	std::string GetActionName() const; 
	void SetActionName(std::string newName);

	std::string GetToolDescription() const;
	void SetToolDescription(std::string newDescription);

	ToolType GetActionType() const;
	void SetToolType(ToolType newToolType);

	bool& IsSelected(); 
	bool& HasVisual(); 

	ActionSelectableInfo GetActionSelectableInfo(); 
 
protected:
	ToolType toolType;

	// Occurrence ----------------
	bool occ_SceneEnter = false; 
	bool occ_SceneLeave = false;
	bool occ_AreaClicked = false;
	bool occ_continuous = false;

	std::string toolName; 
	std::string toolDescription; 
	bool isSelected; 
	bool isVisual; 
}; 

#endif

