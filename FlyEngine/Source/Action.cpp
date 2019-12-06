#include "Action.h"
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

std::string Action::GetActionName() const
{
	return toolName;
}

void Action::SetActionName(std::string newName)
{
	toolName = newName;
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
