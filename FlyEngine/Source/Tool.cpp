#include "Tool.h"
#include "mmgr.h"

Tool::Tool()
{
	toolType = AT_null; 
	isSelected = false; 
	return; 
}

Tool::~Tool()
{

}

void Tool::Init()
{
}

void Tool::Update()
{
}

void Tool::Draw()
{
}

void Tool::CleanUp()
{
}

std::string Tool::GetToolName() const
{
	return toolName;
}

void Tool::SetToolName(std::string newName)
{
	toolName = newName;
}

std::string Tool::GetToolDescription() const
{
	return toolDescription;
}

void Tool::SetToolDescription(std::string newDescription)
{
	toolDescription = newDescription; 
}

ToolType Tool::GetToolType() const
{
	return toolType;
}

void Tool::SetToolType(ToolType newToolType)
{
	toolType = newToolType; 
}

bool& Tool::IsSelected()
{
	return isSelected;
}

bool& Tool::HasVisual()
{
	return isVisual; 
}

ToolSelectableInfo Tool::GetToolSelectableInfo()
{
	ToolSelectableInfo returnToolInfo; 

	returnToolInfo.toolName = toolName; 
	returnToolInfo.toolDescription = toolDescription; 
	returnToolInfo.toolType = toolType; 

	return returnToolInfo;
}
