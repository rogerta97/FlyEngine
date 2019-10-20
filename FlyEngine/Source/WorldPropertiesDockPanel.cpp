#include "WorldPropertiesDockPanel.h"
#include "Application.h"
#include "ModuleWorldManager.h"
#include "ModuleImGui.h"
#include "ModuleInput.h"
#include "imgui.h"
#include "Room.h"
#include "NodeGraph.h"

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

	if (ImGui::Begin(panelName.c_str(), &visible)) {

		PrintRoomsSection();	
		ImGui::Separator();

		if (App->moduleWorldManager->GetSelectedRoom() != nullptr) {
			Room* selectedRoom = App->moduleWorldManager->GetSelectedRoom();
			PrintRoomInfo(selectedRoom);
		}
	}

	ImGui::End();

	return true;
}

void WorldPropertiesDockPanel::PrintRoomInfo(Room* selectedRoom)
{
	std::string colHeaderStr = selectedRoom->GetName() + " Info";

	ImGui::Text("Room Selected: "); ImGui::SameLine();
	ImGui::TextColored(ImVec4(1, 1, 0, 1), "%s", selectedRoom->GetName().c_str());

	ImGui::Text("Enter Connections: "); ImGui::SameLine();
	ImGui::TextColored(ImVec4(1, 1, 0, 1), "%d", selectedRoom->GetInputConnectionsAmount());

	ImGui::Text("Out Connections: "); ImGui::SameLine();
	ImGui::TextColored(ImVec4(1, 1, 0, 1), "%d", selectedRoom->GetOutputConnectionsAmount());

	if (ImGui::CollapsingHeader(colHeaderStr.c_str()))
	{
		PrintOutConnections(selectedRoom);
		PrintEnterConnections(selectedRoom);
	}
}

void WorldPropertiesDockPanel::PrintEnterConnections(Room* selectedRoom)
{
	ImGui::Text("Enter Connections: ");
	ImGui::BeginChild("##EnterConnectionsChild", ImVec2(ImGui::GetWindowContentRegionWidth(), 150), true);

	for (auto it : selectedRoom->inRoomUIDs) {
		if (ImGui::Selectable(App->moduleWorldManager->GetRoom(it)->GetName().c_str())) {
			App->moduleWorldManager->SetSelectedRoom(it);
		}
	}

	ImGui::EndChild();

	// New Connection Button 
	if (ImGui::ImageButton(0, ImVec2(25, 25))) {
		
	}

	// Delete Connection Button
	ImGui::SameLine();
	if (ImGui::ImageButton(0, ImVec2(25, 25))) {


	}
}

void WorldPropertiesDockPanel::PrintOutConnections(Room* selectedRoom)
{
	ImGui::Text("Out Connections: ");

	ImGui::BeginChild("##OutConnectionsChild", ImVec2(ImGui::GetWindowContentRegionWidth(), 150), true);

	for (auto it : selectedRoom->outConnections) {
		if (ImGui::Selectable(it->destinationRoom->GetName().c_str())) {
			App->moduleWorldManager->SetSelectedRoom(it->destinationRoom); 
		}
	}

	if (selectedRoom->GetOutputConnectionsAmount() == 0) {
		ImGui::TextColored(ImVec4(1, 0, 0, 1), "NONE"); 
	}

	ImGui::EndChild();

	// New Connection Button 
	if (ImGui::ImageButton(0, ImVec2(25, 25))) {
		showNewOutConnectionUI = true;
	}

	// Delete Connection Button
	ImGui::SameLine();
	if (ImGui::ImageButton(0, ImVec2(25, 25))) {

	}

	if (showNewOutConnectionUI) {

		ImGui::Separator(); 

		Room* selectedRoom = App->moduleWorldManager->GetSelectedRoom(); 
		static int outConnectionCombo; 
		if (ImGui::Combo("", &outConnectionCombo, App->moduleWorldManager->GetRoomsAsCombo(false))) {

		}
	}
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
//	// Links List
//	ImGui::Text("Connections:");
//	ImGui::BeginChild("RoomsConnectionList", ImVec2(ImGui::GetWindowContentRegionWidth(), ImGui::GetWindowHeight() / 4), true);
//
//	for (auto it : NodeGraph::getInstance()->GetConnectionList()) {
//
//		std::string selectableConnectionText = it->originNode->title.c_str() + std::string(" -> ") + it->destinationNode->title.c_str();
//		if (ImGui::Selectable(selectableConnectionText.c_str(), it->isSelected)) {
//			NodeGraph::getInstance()->connectionSelected = it;
//		}
//	}
//
//	ImGui::EndChild();
//
//	// Add Link Button
//	ImGui::PushFont(App->moduleImGui->buttonFont);
//	if (ImGui::Button("Add Connection", ImVec2(ImGui::GetWindowContentRegionWidth() / 2, 40))) {
//		
//
//	ImGui::SameLine();
//	if (ImGui::Button("Delete Connection", ImVec2(ImGui::GetWindowContentRegionWidth() / 2 - 6, 40))) {
//		if (NodeGraph::getInstance()->connectionSelected != nullptr) {
//			Room* originRoom = App->moduleWorldManager->GetRoom(NodeGraph::getInstance()->connectionSelected->originNode->roomID);
//			originRoom->BreakOutputConnection(NodeGraph::getInstance()->connectionSelected->connectionID);
//		}
//	}
//	ImGui::PopFont();
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
	if (ImGui::ImageButton(0, ImVec2(25, 25))) {
		showNewRoomUI = true;
	}

	// Delete Room Button
	ImGui::SameLine();
	if (ImGui::ImageButton(0, ImVec2(25, 25))) {
		Room* selectedRoom = App->moduleWorldManager->GetSelectedRoom();
		if (selectedRoom != nullptr)
		{
			App->moduleWorldManager->DeleteRoom(selectedRoom->GetRoomID());
			selectedRoom = nullptr;
		}
	}

	if (showNewRoomUI) {
		ShowNewRoomUI();
	}
}

void WorldPropertiesDockPanel::ShowNewRoomUI()
{
	ImGui::Separator();

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
		App->moduleWorldManager->CreateEmptyRoom(newRoomBuffer);
		showNewRoomUI = false;
		newRoomBuffer = "Room Name..."; 
	}
}
