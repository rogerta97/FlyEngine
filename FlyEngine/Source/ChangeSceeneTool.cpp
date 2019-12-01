#include "ChangeSceeneTool.h"

ChangeSceneTool::ChangeSceneTool(FlyObject* _parentObject)
{
	toolType = AT_CHANGE_SCENE;
	isVisual = false;

	SetToolName("Change Scene");
	SetToolDescription("This should be the description of the scene change");
}

ChangeSceneTool::~ChangeSceneTool()
{
}

void ChangeSceneTool::Init()
{
}

void ChangeSceneTool::Update()
{
}

void ChangeSceneTool::CleanUp()
{

}

void ChangeSceneTool::SetDestination(Room* dstRoom)
{
	destinationRoom = dstRoom; 
}

Room* ChangeSceneTool::GetDesination()
{
	return destinationRoom;
}
