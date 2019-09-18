#include "GraphPropertiesDockPanel.h"
#include "Application.h"
#include "ModuleImGui.h"
#include "ModuleWorldManager.h"
#include "Room.h"
#include "NodeGraph.h"
#include "imgui.h"

#include "mmgr.h"

GraphPropertiesDockPanel::GraphPropertiesDockPanel(bool isVisible) : DockPanel("Graph Properties", isVisible)
{
	flyEngineSection = FLY_SECTION_ROOM_GRAPH;
	dockPanelType = DOCK_GAME_VIEWPORT;	
}

GraphPropertiesDockPanel::~GraphPropertiesDockPanel()
{
}

bool GraphPropertiesDockPanel::Draw()
{

#pragma region secutiryChecks
	if (!DockPanel::Draw())
		return false; 
#pragma endregion

	if (ImGui::Begin(panelName.c_str(), &visible)) {

		// World Properties
		if(ImGui::CollapsingHeader("World Properties")) {

			PrintRoomsSection();
			ImGui::Separator();
			ImGui::Separator();

			PrintConnectionsSection();
			ImGui::Separator();
			ImGui::Separator();

			// Show Naming Popup
			NewRoomButtonHandler();
			NewConnectionButtonHandler();
		}

		// Room Properties 
		if (ImGui::CollapsingHeader("Room Properties")) {

		}

		// Node Properties 
		if (ImGui::CollapsingHeader("Node Properties")) {
			
		}
	}

	ImGui::End();
	return true;
}

void GraphPropertiesDockPanel::NewConnectionButtonHandler()
{
	if (ImGui::BeginPopup("new_connection_popup")) {

		static int originRoomSelected = 0;
		static int destinationRoomSelected = 0;

		ImGui::Combo("Origin Room", &originRoomSelected, NodeGraph::getInstance()->GetNodesAsCombo().c_str());
		ImGui::Combo("Destination Room", &originRoomSelected, NodeGraph::getInstance()->GetConnectionsAsCombo().c_str());

		if (ImGui::Button("Connect")) {
			ImGui::CloseCurrentPopup();
		}

		ImGui::EndPopup();
	}
}

void GraphPropertiesDockPanel::PrintConnectionsSection()
{
	// Links List
	ImGui::Text("Connections:");
	ImGui::BeginChild("RoomsConnectionList", ImVec2(ImGui::GetWindowContentRegionWidth(), ImGui::GetWindowHeight() / 4), true);

	//for (int n = 0; n < 5; n++)
	//{
	//	char buf[32];
	//	sprintf(buf, "Object %d", n);
	//	if (ImGui::Selectable(buf, selection[n]))
	//	{
	//		if (!ImGui::GetIO().KeyCtrl)    // Clear selection when CTRL is not held
	//			memset(selection, 0, sizeof(selection));
	//		selection[n] ^= 1;
	//	}
	//}

	for (auto it : App->moduleWorldManager->connectionsInWorldList) {
		string selectableConnectionText = it->originRoom->GetName() + string(" -> ") + it->destinationRoom->GetName();
		ImGui::Selectable(selectableConnectionText.c_str());
	}

	ImGui::EndChild();

	// Add Link Button
	ImGui::PushFont(App->moduleImGui->buttonFont);
	if (ImGui::Button("Add Connection", ImVec2(ImGui::GetWindowContentRegionWidth() / 2, 40))) {
		ImGui::OpenPopup("new_connection_popup");
	}

	ImGui::SameLine();
	if (ImGui::Button("Delete Connection", ImVec2(ImGui::GetWindowContentRegionWidth() / 2 - 6, 40))) {

	}
	ImGui::PopFont();
}

void GraphPropertiesDockPanel::PrintRoomsSection()
{
	ImGui::Text("Rooms:");
	ImGui::BeginChild("RoomsList", ImVec2(ImGui::GetWindowContentRegionWidth(), ImGui::GetWindowHeight() / 4), true);

	for (auto it : App->moduleWorldManager->roomsInWorldList) {
		ImGui::Selectable(it->GetName().c_str()); 
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

	}
	ImGui::PopFont();
}

void GraphPropertiesDockPanel::NewRoomButtonHandler()
{
	if (ImGui::BeginPopup("new_room_popup")) {
		static char newRoomBuffer[64] = "Room Name...";
		ImGui::InputText("##2", (char*)newRoomBuffer, 64 * sizeof(char));

		ImGui::SameLine();
		if (ImGui::Button("OK")) {
			App->moduleWorldManager->CreateEmptyRoom(newRoomBuffer);
			ImGui::CloseCurrentPopup(); 
		}

		ImGui::EndPopup();
	}
}
