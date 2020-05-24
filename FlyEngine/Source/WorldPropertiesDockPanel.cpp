#include "WorldPropertiesDockPanel.h"
#include "Application.h"
#include "ModuleWorldManager.h"
#include "Texture.h"
#include "ResourceManager.h"
#include "ModuleImGui.h"
#include "ModuleInput.h"
#include "imgui.h"
#include "TextureMSAA.h"
#include "GameViewportDockPanel.h"
#include "SaveAndLoad.h"
#include "ViewportManager.h"
#include "Room.h"
#include "NodeGraph.h"
#include "mmgr.h"

WorldPropertiesDockPanel::WorldPropertiesDockPanel(bool isVisible) : DockPanel("World Properties", isVisible)
{
	flyEngineSection = FLY_SECTION_ROOM_GRAPH;
	dockPanelType = DOCK_WORLD_PROPERTIES;
	newRoomBuffer = "New Room...";
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

		if (App->moduleWorldManager->GetSelectedRoom() != nullptr) 
		{
			Room* selectedRoom = App->moduleWorldManager->GetSelectedRoom();
			PrintRoomInfo(selectedRoom);
		}
	}

	ImGui::End();

	return true;
}

void WorldPropertiesDockPanel::PrintRoomInfo(Room* selectedRoom)
{
	/*if(ImGui::CollapsingHeader("Room Info", ImGuiTreeNodeFlags_DefaultOpen)) {

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
	}*/
}

void WorldPropertiesDockPanel::NewConnectionButtonHandler()
{
	if (ImGui::BeginPopup("new_connection_popup")) {

		static int originRoomSelected = 0;
		static int destinationRoomSelected = 0;

	/*	ImGui::Combo("Origin Room", &originRoomSelected, NodeGraph::getInstance()->GetNodesAsCombo().c_str());
		ImGui::Combo("Destination Room", &destinationRoomSelected, NodeGraph::getInstance()->GetNodesAsCombo().c_str());*/

		if (ImGui::Button("Connect")) 
		{
			list<Room*>::iterator itOrigin = std::next(App->moduleWorldManager->roomsInWorldList.begin(), originRoomSelected);
			list<Room*>::iterator itDst = std::next(App->moduleWorldManager->roomsInWorldList.begin(), destinationRoomSelected);

			Room* originRoom = (*itOrigin);
			Room* dstRoom = (*itDst);

			originRoom->AddConnection(dstRoom);

			ImGui::CloseCurrentPopup();
		}

		ImGui::EndPopup();
	}
}

void WorldPropertiesDockPanel::PrintRoomsSection()
{
	ImGui::PushFont(App->moduleImGui->rudaBlackHuge);
	ImGui::Text("Rooms:");
	ImGui::PopFont();
	
	PUSH_CHILD_BG_COLOR;
	ImGui::BeginChild("RoomsList", ImVec2(ImGui::GetWindowContentRegionWidth(), 250), true);

	ImGui::PushFont(App->moduleImGui->rudaBlackMid);
	for (auto currentRoom : App->moduleWorldManager->GetRoomsInWorldList()) {

		if (ImGui::Selectable(currentRoom->GetName().c_str(), currentRoom->isSelected)) 
		{
			App->moduleWorldManager->SetSelectedRoom(currentRoom->GetUID());
		}
	}
	ImGui::PopFont(); 

	ImGui::EndChild();
	ImGui::PopStyleColor();

	// New Room Button 
	if (ImGui::Button("Add Room")) {
		newRoomBuffer = "New Room...";
		ImGui::OpenPopup("new_room"); 
	}

	ShowNewRoomUI();

	// Delete Room Button
	ImGui::SameLine();
	if (ImGui::Button("Delete Room")) {
		Room* selectedRoom = App->moduleWorldManager->GetSelectedRoom();
		if (selectedRoom != nullptr)
		{
			App->moduleWorldManager->DeleteRoom(selectedRoom->GetUID());
			selectedRoom = nullptr;
		}
	}
	
	ImGui::SameLine();
	ImGui::SetCursorPosX(ImGui::GetContentRegionMax().x - 45); 
	Texture* editIcon = (Texture*)ResourceManager::getInstance()->GetResource("EditIcon"); 
	if (ImGui::ImageButton((ImTextureID)editIcon->GetTextureID(), ImVec2(40, 40))) 
	{
		if (App->moduleWorldManager->GetSelectedRoom() != nullptr)
		{
			App->moduleImGui->AddaptToFlySection(FlyEngineSection::FLY_SECTION_ROOM_EDIT);
			App->moduleImGui->gameViewportDockPanel->FitViewportToRegion();
		}
	}
}

void WorldPropertiesDockPanel::ShowNewRoomUI()
{
	if (ImGui::BeginPopup("new_room")) 
	{
		bool createThisTick = false;
		ImGui::InputText("##2", (char*)newRoomBuffer.c_str(), 265 * sizeof(char));

		if (App->moduleInput->GetKey(SDL_SCANCODE_RETURN) == KEY_DOWN || App->moduleInput->GetKey(SDL_SCANCODE_KP_ENTER) == KEY_DOWN) {
			createThisTick = true;
		}

		ImGui::SameLine();

		if (ImGui::Button("Create")) {
			createThisTick = true;
		}

		if (createThisTick) 
		{
			Room* newRoom = App->moduleWorldManager->CreateEmptyRoom(newRoomBuffer);
			SaveAndLoad::getInstance()->SaveRoomData(newRoom); 

			ImGui::CloseCurrentPopup();		
		}

		ImGui::EndPopup();
	}

	
	
}
