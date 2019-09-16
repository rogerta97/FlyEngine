#include "GraphPropertiesDockPanel.h"
#include "Application.h"
#include "ModuleWorldManager.h"
#include "Room.h"
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

		ImGui::Text("Rooms in world:"); 
		ImGui::BeginChild("RoomsList", ImVec2(ImGui::GetWindowContentRegionWidth(), ImGui::GetWindowHeight() / 2), true);

		for (auto it : App->moduleWorldManager->roomsInWorldList) {
			ImGui::Selectable(it->GetName().c_str());
		}

		ImGui::EndChild();
		
		static char newRoomBuffer[256] = "Hello"; 
		ImGui::InputText("##2", (char*)newRoomBuffer, 256 * sizeof(char));
		
		ImGui::SameLine();
		if (ImGui::Button("Create New Room")) {
			App->moduleWorldManager->CreateEmptyRoom(newRoomBuffer); 
		}

		ImGui::Spacing();
		ImGui::Separator();
		ImGui::Separator();
		ImGui::Spacing(); 

		if (ImGui::CollapsingHeader("Node Properties")) {
			
		}
	}

	ImGui::End();
	return true;
}
