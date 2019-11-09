#include "Tool.h"

Tool::Tool()
{
	toolType = AT_null; 
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
