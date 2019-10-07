#include "WorldPropertiesDockPanel.h"
#include "Application.h"
#include "ModuleWorldManager.h"
#include "ModuleImGui.h"
#include "ModuleInput.h"
#include "imgui.h"
#include "Room.h"
#include "NodeGraph.h"

WorldPropertiesDockPanel::WorldPropertiesDockPanel(bool isVisible) : DockPanel("World Management", isVisible)
{
	flyEngineSection = FLY_SECTION_ROOM_GRAPH;
	dockPanelType = DOCK_WORLD_PROPERTIES;
}

WorldPropertiesDockPanel::~WorldPropertiesDockPanel()
{
}

bool WorldPropertiesDockPanel::Draw()
{

	if (ImGui::Begin(panelName.c_str(), &visible)) {

		PrintRoomsSection();
	
		ImGui::Separator();

		//PrintConnectionsSection();
		//ImGui::Separator();
		//ImGui::Separator();

		// Show Naming Popup
		NewRoomButtonHandler();
		//NewConnectionButtonHandler();

		if (App->moduleWorldManager->GetSelectedRoom() != nullptr) {

			Room* selectedRoom = App->moduleWorldManager->GetSelectedRoom();
			std::string colHeaderStr = "Room Info [" + selectedRoom->GetName() + "]";
			if (ImGui::CollapsingHeader(colHeaderStr.c_str())) {
				
				ImGui::Text("Out Connections: "); 

				ImGui::BeginChild("##OutConnectionsChild", ImVec2(ImGui::GetWindowContentRegionWidth(), ImGui::GetWindowHeight() / 4), true);

				for (auto it : selectedRoom->outConnections) {
					if (ImGui::Selectable(it->destinationRoom->GetName().c_str())) {

					}
				}

				ImGui::EndChild(); 		

			}
		}
	}

	ImGui::End();

	return true;
}

void WorldPropertiesDockPanel::NewConnectionButtonHandler()
{
	if (ImGui::BeginPopup("new_connection_popup")) {

		static int originRoomSelected = 0;
		static int destinationRoomSelected = 0;

		ImGui::Combo("Origin Room", &originRoomSelected, NodeGraph::getInstance()->GetNodesAsCombo().c_str());
		ImGui::Combo("Destination Room", &destinationRoomSelected, NodeGraph::getInstance()->GetNodesAsCombo().c_str());

		if (ImGui::Button("Connect")) {


			list<Room*>::iterator itOrigin = std::next(App->moduleWorldManager->roomsInWorldList.begin(), originRoomSelected);
			list<Room*>::iterator itDst = std::next(App->moduleWorldManager->roomsInWorldList.begin(), destinationRoomSelected);

			Room* originRoom = (*itOrigin);
			Room* dstRoom = (*itDst);

			originRoom->ConnectToRoom(dstRoom);

			ImGui::CloseCurrentPopup();
		}

		ImGui::EndPopup();
	}
}

void WorldPropertiesDockPanel::PrintConnectionsSection()
{
	// Links List
	ImGui::Text("Connections:");
	ImGui::BeginChild("RoomsConnectionList", ImVec2(ImGui::GetWindowContentRegionWidth(), ImGui::GetWindowHeight() / 4), true);

	for (auto it : NodeGraph::getInstance()->GetConnectionList()) {

		std::string selectableConnectionText = it->originNode->title.c_str() + std::string(" -> ") + it->destinationNode->title.c_str();
		if (ImGui::Selectable(selectableConnectionText.c_str(), it->isSelected)) {
			NodeGraph::getInstance()->connectionSelected = it;
		}
	}

	ImGui::EndChild();

	// Add Link Button
	ImGui::PushFont(App->moduleImGui->buttonFont);
	if (ImGui::Button("Add Connection", ImVec2(ImGui::GetWindowContentRegionWidth() / 2, 40))) {
		ImGui::OpenPopup("new_connection_popup");
	}

	ImGui::SameLine();
	if (ImGui::Button("Delete Connection", ImVec2(ImGui::GetWindowContentRegionWidth() / 2 - 6, 40))) {
		if (NodeGraph::getInstance()->connectionSelected != nullptr) {
			Room* originRoom = App->moduleWorldManager->GetRoom(NodeGraph::getInstance()->connectionSelected->originNode->roomID);
			originRoom->BreakOutputConnection(NodeGraph::getInstance()->connectionSelected->connectionID);
		}
	}
	ImGui::PopFont();
}

void WorldPropertiesDockPanel::PrintRoomsSection()
{
	ImGui::Text("Rooms:");
	ImGui::BeginChild("RoomsList", ImVec2(ImGui::GetWindowContentRegionWidth(), ImGui::GetWindowHeight() / 4), true);

	for (auto it : NodeGraph::getInstance()->GetNodeList()) {

		if (ImGui::Selectable(it->title.c_str(), &it->selected)) {
			App->moduleWorldManager->SetSelectedRoom(it->title);
		}
	}

	ImGui::EndChild();

	// New Room Button 
	ImGui::PushFont(App->moduleImGui->buttonFont);
	if (ImGui::Button("Add Room", ImVec2(ImGui::GetWindowContentRegionWidth() / 2, 40))) {
		ImGui::OpenPopup("new_room_popup");
	}

	// Delete Room Button
	ImGui::SameLine();
	if (ImGui::Button("Delete Room", ImVec2((ImGui::GetWindowContentRegionWidth() / 2) - 7, 40))) {
		Room* selectedRoom = App->moduleWorldManager->GetSelectedRoom();
		if (selectedRoom != nullptr)
		{
			App->moduleWorldManager->DeleteRoom(selectedRoom->GetRoomID());
			selectedRoom = nullptr;
		}
	}
	ImGui::PopFont();
}

void WorldPropertiesDockPanel::NewRoomButtonHandler()
{
	if (ImGui::BeginPopup("new_room_popup")) {

		static char newRoomBuffer[64] = "...";
		ImGui::InputText("##2", (char*)newRoomBuffer, 64 * sizeof(char));

		if (App->moduleInput->GetKey(SDL_SCANCODE_RETURN) == KEY_DOWN || App->moduleInput->GetKey(SDL_SCANCODE_KP_ENTER) == KEY_DOWN) {
			App->moduleWorldManager->CreateEmptyRoom(newRoomBuffer);
			ImGui::CloseCurrentPopup();
		}

		ImGui::SameLine();

		if (ImGui::Button("polla")) {
			App->moduleWorldManager->CreateEmptyRoom(newRoomBuffer);
			ImGui::CloseCurrentPopup();
		}

		ImGui::EndPopup();
	}
}
