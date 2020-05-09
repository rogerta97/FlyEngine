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
	projectName = ""; 
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

		static char tmpProjectName[256] = "";

		if (!projectName.empty())
			strcpy(tmpProjectName, projectName.c_str()); 

		if (ImGui::InputTextWithHint("Project Name", "Name...", tmpProjectName, IM_ARRAYSIZE(tmpProjectName)))
		{
			projectName = tmpProjectName;
		}

		if (ImGui::Button("Save As Release"))
		{
			ImGui::OpenPopup("save_as_release"); 
		}

		if (ImGui::BeginPopupModal("save_as_release"))
		{
			ImGui::SetWindowSize(ImVec2(330, 120));
			ImGui::PushFont(App->moduleImGui->rudaRegularTiny);
			ImGui::TextWrapped("Fly Engine will go into Game Mode and it needs to be shut down. Next time it's executed menus will be innacessible.");
			ImGui::PopFont();

			ImGui::SetCursorPosX(ImGui::GetContentRegionMax().x / 2 - 50);
			if (ImGui::Button("Go"))
			{
				App->SetGameMode(PLAY_MODE); 
				App->Exit();
				ImGui::CloseCurrentPopup();
			}

			ImGui::SameLine();
			if (ImGui::Button("Cancel"))
			{
				ImGui::CloseCurrentPopup();
			}

			ImGui::EndPopup();
		}
	}

	ImGui::End();
	return true;
}

std::string GamePropertiesDockPanel::GetProjectName()
{
	return projectName;
}


