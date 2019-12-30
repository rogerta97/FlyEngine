#include "ChangeRoomAction.h"
#include "imgui.h"
#include "FlyObject.h"

#include "Application.h"
#include "ModuleImGui.h"
#include "Room.h"
#include "ModuleRoomManager.h"

#include "mmgr.h"

ChangeRoomAction::ChangeRoomAction(FlyObject* _parentObject)
{
	actionType = AT_CHANGE_ROOM;
	isVisual = false;

	SetActionName("Change Room");
	SetToolDescription("This should be the description of the scene change");
}

ChangeRoomAction::~ChangeRoomAction()
{
}

void ChangeRoomAction::DrawActionOccurenceCheckboxes()
{
	ImGui::PushFont(App->moduleImGui->rudaBoldBig);
	ImGui::Text("Action Happens On:");
	ImGui::PopFont();

	ImGui::PushFont(App->moduleImGui->rudaRegularMid);
	ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0.12f, 0.14f, 0.17f, 1.00f));
	ImGui::BeginChild("##OccChild", ImVec2(ImGui::GetContentRegionAvailWidth(), 35));

	ImGui::Checkbox("Object Clicked", &occ_ObjectClicked);

	ImGui::Spacing(); 
	ImGui::EndChild(); 

	ImGui::PopFont();
	ImGui::PopStyleColor(); 
}

void ChangeRoomAction::Init()
{
}

void ChangeRoomAction::Update()
{
}

void ChangeRoomAction::CleanUp()
{
	Action::CleanUp();
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
