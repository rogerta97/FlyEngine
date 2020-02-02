#include "WorldPropertiesDockPanel.h"
#include "Application.h"
#include "ModuleRoomManager.h"
#include "ModuleImGui.h"
#include "ModuleInput.h"
#include "imgui.h"
#include "Room.h"
#include "NodeGraph.h"
#include "mmgr.h"

WorldPropertiesDockPanel::WorldPropertiesDockPanel(bool isVisible) : DockPanel("World Properties", isVisible)
{
	flyEngineSection = FLY_SECTION_ROOM_GRAPH;
	dockPanelType = DOCK_WORLD_PROPERTIES;
}

WorldPropertiesDockPanel::~WorldPropertiesDockPanel()
{
}

bool WorldPropertiesDockPanel::Draw()
{
#pragma region secutiryChecks
	if (!DockPanel::Draw())
		return false;
#pragma endregion

	if (ImGui::Begin(panelName.c_str(), &visible)) {

		PrintRoomsSection();

		ImGui::Separator();
		ImGui::Separator(); 

		if (App->moduleRoomManager->GetSelectedRoom() != nullptr) {
			Room* selectedRoom = App->moduleRoomManager->GetSelectedRoom();
			PrintRoomInfo(selectedRoom);
		}
	}

	ImGui::End();

	return true;
}

void WorldPropertiesDockPanel::PrintRoomInfo(Room* selectedRoom)
{
	if(ImGui::CollapsingHeader("Room Info")) {

		ImGui::Text("Room Selected: "); ImGui::SameLine();
		ImGui::TextColored(ImVec4(1, 1, 0, 1), "%s", selectedRoom->GetName().c_str());

		ImGui::Text("Output Connections: "); ImGui::SameLine();
		ImGui::TextColored(ImVec4(1, 1, 0, 1), "%d", selectedRoom->GetOutputConnectionsAmount());

		ImGui::Text("Input Connections: "); ImGui::SameLine();
		ImGui::TextColored(ImVec4(1, 1, 0, 1), "%d", selectedRoom->GetEnterConnectionsAmount());
		
		ImGui::Separator();

		if (selectedRoom->GetOutputConnectionsAmount() > 0) {

			
			if (ImGui::TreeNode("Out Connections")) {

				for (auto& it : selectedRoom->outConnections) {
					ImGui::Selectable((it)->destinationRoom->GetName().c_str()); 
				}

				ImGui::TreePop(); 
			}
		}

		if (selectedRoom->GetEnterConnectionsAmount() > 0) {

			if (ImGui::TreeNode("Enter Connections")) {

				for (auto& it : selectedRoom->inRoomUIDs) {
					ImGui::Selectable(App->moduleRoomManager->GetRoom(it)->GetName().c_str());
				}

				ImGui::TreePop();
			}
		}

		if (ImGui::Button("New Connection")) {

		}

		ImGui::Separator(); 
	}
}

void WorldPropertiesDockPanel::NewConnectionButtonHandler()
{
	if (ImGui::BeginPopup("new_connection_popup")) {

		static int originRoomSelected = 0;
		static int destinationRoomSelected = 0;

		ImGui::Combo("Origin Room", &originRoomSelected, NodeGraph::getInstance()->GetNodesAsCombo().c_str());
		ImGui::Combo("Destination Room", &destinationRoomSelected, NodeGraph::getInstance()->GetNodesAsCombo().c_str());

		if (ImGui::Button("Connect")) 
		{
			list<Room*>::iterator itOrigin = std::next(App->moduleRoomManager->roomsInWorldList.begin(), originRoomSelected);
			list<Room*>::iterator itDst = std::next(App->moduleRoomManager->roomsInWorldList.begin(), destinationRoomSelected);

			Room* originRoom = (*itOrigin);
			Room* dstRoom = (*itDst);

			originRoom->ConnectToRoom(dstRoom);

			ImGui::CloseCurrentPopup();
		}

		ImGui::EndPopup();
	}
}

void WorldPropertiesDockPanel::PrintRoomsSection()
{
	ImGui::Text("Rooms:");
	
	ImGui::BeginChild("RoomsList", ImVec2(ImGui::GetWindowContentRegionWidth(), ImGui::GetWindowHeight() / 4), true);

	for (auto it : NodeGraph::getInstance()->GetNodeList()) {

		if (ImGui::Selectable(it->title.c_str(), &it->selected)) {
			App->moduleRoomManager->SetSelectedRoom(it->title);
		}
	}

	ImGui::EndChild();

	// New Room Button 
	if (ImGui::Button("New")) {
		ImGui::OpenPopup("new_room"); 
	}

	ShowNewRoomUI();

	// Delete Room Button
	ImGui::SameLine();
	if (ImGui::Button("Delete")) {
		Room* selectedRoom = App->moduleRoomManager->GetSelectedRoom();
		if (selectedRoom != nullptr)
		{
			App->moduleRoomManager->DeleteRoom(selectedRoom->GetUID());
			selectedRoom = nullptr;
		}
	}

	ImGui::SameLine(ImGui::GetContentRegionMax().x - 130);
	if (ImGui::Button("Open Room Editor", ImVec2(130, 30))) 
	{
		if(App->moduleRoomManager->GetSelectedRoom() != nullptr)
			App->moduleImGui->AddaptToFlySection(FlyEngineSection::FLY_SECTION_ROOM_EDIT);
	}
}

void WorldPropertiesDockPanel::ShowNewRoomUI()
{
	if (ImGui::BeginPopup("new_room")) 
	{
		bool createThisTick = false;
		string newRoomBuffer("Room Name...");
		ImGui::InputText("##2", (char*)newRoomBuffer.c_str(), 265 * sizeof(char));

		if (App->moduleInput->GetKey(SDL_SCANCODE_RETURN) == KEY_DOWN || App->moduleInput->GetKey(SDL_SCANCODE_KP_ENTER) == KEY_DOWN) {
			createThisTick = true;
		}

		ImGui::SameLine();

		if (ImGui::Button("Create")) {
			createThisTick = true;
		}

		if (createThisTick) {
			App->moduleRoomManager->CreateEmptyRoom(newRoomBuffer);
			newRoomBuffer = "Room Name...";
			ImGui::CloseCurrentPopup();		
		}

		ImGui::EndPopup();
	}

	
	
}
