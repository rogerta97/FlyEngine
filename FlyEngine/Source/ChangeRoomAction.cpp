#include "ChangeRoomAction.h"
#include "imgui.h"
#include "FlyObject.h"

#include "Application.h"
#include "ModuleImGui.h"
#include "ModuleRoomManager.h"

ChangeRoomAction::ChangeRoomAction(FlyObject* _parentObject)
{
	toolType = AT_CHANGE_ROOM;
	isVisual = false;

	SetActionName("Change Scene");
	SetToolDescription("This should be the description of the scene change");
}

ChangeRoomAction::~ChangeRoomAction()
{
}

void ChangeRoomAction::DrawActionOccurenceCheckboxes()
{
	
}

void ChangeRoomAction::Init()
{
}

void ChangeRoomAction::Update()
{
}

void ChangeRoomAction::CleanUp()
{

}

void ChangeRoomAction::DoAction()
{
	if(destinationRoom)
		App->moduleRoomManager->SetSelectedRoom(destinationRoom);
}

void ChangeRoomAction::SetDestination(Room* dstRoom)
{
	destinationRoom = dstRoom; 
}

Room* ChangeRoomAction::GetDesination()
{
	return destinationRoom;
}
