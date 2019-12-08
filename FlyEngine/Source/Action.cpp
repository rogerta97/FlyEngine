#include "Action.h"
#include "FlyObject.h"

#include "mmgr.h"

Action::Action()
{
	toolType = AT_null; 
	isSelected = false; 
	return; 
}

Action::~Action()
{

}

void Action::Init()
{
}

void Action::Update()
{
}

void Action::Draw()
{
}

void Action::CleanUp()
{
}

void Action::SaveAction(JSON_Object* jsonObject, std::string serializeObjectString)
{
}

void Action::DoAction()
{
	
}

std::string Action::GetActionName() const
{
	return toolName;
}

void Action::SetActionName(std::string newName)
{
	toolName = newName;
}

bool& Action::IsOccSceneEnter()
{
	return occ_SceneEnter; 
}

bool& Action::IsOccSceneLeave()
{
	return occ_SceneLeave;
}

bool& Action::IsOccObjectClicked()
{
	return occ_ObjectClicked;
}

void Action::SetOccSceneEnter(bool newOccSceneEnter)
{
	occ_SceneEnter = newOccSceneEnter; 
}

void Action::SetOccSceneLeave(bool newOccSceneLeave)
{
	occ_SceneLeave = newOccSceneLeave;
}

void Action::SetOccObjectClicked(bool newOccObjectClicked)
{
	occ_ObjectClicked = newOccObjectClicked;
}

FlyObject* Action::GetParentObject() const
{
	return parentObject;
}

void Action::SetParentObject(FlyObject* newParentObject)
{
	parentObject = newParentObject; 
}

std::string Action::GetToolDescription() const
{
	return toolDescription;
}

void Action::SetToolDescription(std::string newDescription)
{
	toolDescription = newDescription; 
}

ToolType Action::GetActionType() const
{
	return toolType;
}

void Action::SetToolType(ToolType newToolType)
{
	toolType = newToolType; 
}

bool& Action::IsSelected()
{
	return isSelected;
}

bool& Action::HasVisual()
{
	return isVisual; 
}

ActionSelectableInfo Action::GetActionSelectableInfo()
{
	ActionSelectableInfo returnToolInfo; 

	returnToolInfo.actionName = toolName; 
	returnToolInfo.actionDescription = toolDescription; 
	returnToolInfo.actionType = toolType; 

	return returnToolInfo;
}
