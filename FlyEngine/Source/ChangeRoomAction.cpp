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

void ChangeRoomAction::Update()
{
}

void ChangeRoomAction::CleanUp()
{
	Action::CleanUp();
}

void ChangeRoomAction::DrawSelectDestinationCombo()
{
	/*const char* roomsArrayCombo[MAX_ROOMS]; 
	
	std::list<Room*> roomsInWorld = App->moduleRoomManager->GetRoomsInWorldList();

	int count = 0; 
	for (auto& currentRoom : roomsInWorld)
	{
		roomsArrayCombo[count] = currentRoom->GetName().c_str(); 
	}*/
}

void ChangeRoomAction::DrawUISettings()
{
	ImGui::PushFont(App->moduleImGui->rudaBoldBig);
	ImGui::Text("Change Room Settings: ");
	ImGui::PopFont();

	const char** rooms = App->moduleRoomManager->GetRoomsAsCombo();
	const char* roomsArr[] = {"Hello", "Bye"};

	//roomsArr[0] = "This"; 
	//roomsArr[1] = "Is"; 
	//roomsArr[2] = "A"; 
	//roomsArr[3] = "Test"; 

	static int ci = 0;
	ImGui::ComboArray("Destination", &ci, roomsArr, IM_ARRAYSIZE(roomsArr));
}

void ChangeRoomAction::DrawUISettingsInButton()
{
	ImGui::PushFont(App->moduleImGui->rudaBoldBig);
	ImGui::Text("Change Room Settings: ");
	ImGui::PopFont();

	const char** rooms = App->moduleRoomManager->GetRoomsAsCombo();
	const char* roomsToCombo[] = { "None", *rooms, *rooms + 1, *rooms + 2 };
	static int ci = 0;
	ImGui::ComboArray("Destination", &ci, roomsToCombo, IM_ARRAYSIZE(roomsToCombo));
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
