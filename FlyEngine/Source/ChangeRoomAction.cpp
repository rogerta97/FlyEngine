#include "ChangeRoomAction.h"
#include "imgui.h"
#include "FlyObject.h"

#include "Application.h"
#include "ModuleImGui.h"
#include "Room.h"
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

void ChangeRoomAction::SaveAction(JSON_Object* jsonObject, string serializeObjectString)
{
	string toolsSerializeSection = serializeObjectString + string("Actions.ChangeRoom.");

	if (destinationRoom != nullptr)
		json_object_dotset_string(jsonObject, string(toolsSerializeSection + string("Destination")).c_str(), destinationRoom->GetName().c_str());
	else
		json_object_dotset_string(jsonObject, string(toolsSerializeSection + string("Destination")).c_str(), "None");
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
