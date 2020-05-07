#include "GamePropertiesDockPanel.h"
#include "Application.h"
#include "ModuleImGui.h"
#include "ModuleInput.h"
#include "ModuleRoomManager.h"
#include "Room.h"
#include "NodeGraph.h"
#include "imgui.h"

#include "mmgr.h"
#include <string>

GamePropertiesDockPanel::GamePropertiesDockPanel(bool isVisible) : DockPanel("Game Properties", isVisible)
{
	flyEngineSection = FLY_SECTION_ROOM_GRAPH;
	dockPanelType = DOCK_GAME_PROPERTIES;
	firstRoom = nullptr; 
}

GamePropertiesDockPanel::~GamePropertiesDockPanel()
{
}

bool GamePropertiesDockPanel::Draw()
{
#pragma region secutiryChecks
	if (!DockPanel::Draw())
		return false; 
#pragma endregion

	if (ImGui::Begin(panelName.c_str(), &visible)) 
	{
		static char roomShowingName[256] = "Room Name..."; 

		if (ImGui::BeginCombo("Start Room", roomShowingName))
		{
			int count = 0;
			for (auto& currentRoom : App->moduleRoomManager->roomsInWorldList)
			{
				if (ImGui::Selectable(currentRoom->GetName().c_str()))
				{
					App->moduleRoomManager->SetStartRoom(currentRoom); 
					strcpy(roomShowingName, currentRoom->GetName().c_str());
				}
			}
			ImGui::EndCombo();
		}
	}

	ImGui::End();
	return true;
}


