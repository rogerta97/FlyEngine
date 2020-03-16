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
	actionType = ACTION_CHANGE_ROOM;
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

void ChangeRoomAction::Update(float dt)
{
}

void ChangeRoomAction::CleanUp()
{
	Action::CleanUp();
}

void ChangeRoomAction::DrawSelectDestinationCombo()
{
	std::list<Room*> roomsInWorld = App->moduleRoomManager->GetRoomsInWorldList();

	char selectedRoom[256] = "Select Destination...";
	if (GetDesination() != nullptr)
	{
		strcpy(selectedRoom, GetDesination()->GetName().c_str());
	}

	if (ImGui::BeginCombo("Destination", selectedRoom))
	{
		int count = 0;
		for (auto& currentRoom : roomsInWorld)
		{
			if (ImGui::Selectable(currentRoom->GetName().c_str()))
				SetDestination(currentRoom);
		}
		ImGui::EndCombo();
	}
}

void ChangeRoomAction::DrawUISettings()
{
	ImGui::PushFont(App->moduleImGui->rudaBoldBig);
	ImGui::Text("Change Room Settings: ");
	ImGui::PopFont();

	DrawSelectDestinationCombo();
}

void ChangeRoomAction::DrawUISettingsInButton()
{
	ImGui::PushFont(App->moduleImGui->rudaBoldBig);
	ImGui::Text("Change Room Settings: ");
	ImGui::PopFont();

	DrawSelectDestinationCombo(); 
}

void ChangeRoomAction::SaveAction(JSON_Object* jsonObject, string serializeObjectString, bool literalStr)
{
	string toolsSerializeSection;
	
	if (!literalStr)
		toolsSerializeSection = serializeObjectString + string("Actions.ChangeRoom.");
	else
		toolsSerializeSection = serializeObjectString; 

	Action::SaveAction(jsonObject, toolsSerializeSection);
	Action::SaveOccurrence(jsonObject, toolsSerializeSection);

	if (destinationRoom != nullptr)
		json_object_dotset_number(jsonObject, string(toolsSerializeSection + string("Destination")).c_str(), destinationRoom->GetUID());
	else
		json_object_dotset_number(jsonObject, string(toolsSerializeSection + string("Destination")).c_str(), 0);
}

void ChangeRoomAction::DoAction()
{
	if (destinationRoom)
	{
		App->moduleRoomManager->SetSelectedRoom(destinationRoom);
		SetActionCompleted(true); 
	}
}

void ChangeRoomAction::SetDestination(Room* dstRoom)
{
	destinationRoom = dstRoom; 
}

Room* ChangeRoomAction::GetDesination()
{
	return destinationRoom;
}
