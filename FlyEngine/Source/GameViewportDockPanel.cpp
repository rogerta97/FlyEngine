#include "GameViewportDockPanel.h"
#include "imgui.h"
#include "Application.h"
#include "Room.h"
#include "ModuleWorldManager.h"
#include "mmgr.h"

GameViewportDockPanel::GameViewportDockPanel(bool isVisible) : DockPanel("Game Viewport", isVisible)
{
	flyEngineSection = FLY_SECTION_ROOM_EDIT;
	dockPanelType = DOCK_GAME_VIEWPORT;
}

GameViewportDockPanel::~GameViewportDockPanel()
{
}

bool GameViewportDockPanel::Draw()
{

#pragma region secutiryChecks
	if (!DockPanel::Draw())
		return false;
#pragma endregion

	if (ImGui::Begin(panelName.c_str(), &visible, ImGuiWindowFlags_MenuBar)) {
		
		if (ImGui::BeginMenuBar()) 
		{
			if (ImGui::Button("Create Object")) {
				ImGui::OpenPopup("create_flyobject"); 
			}

			ObjectCreatorPopup();

			ImGui::EndMenuBar(); 
		}

		ImGui::Image(0, ImVec2(ImGui::GetWindowSize().x, ImGui::GetWindowSize().y - 60));
	}

	ImGui::End();
	return true; 
}

void GameViewportDockPanel::ObjectCreatorPopup()
{
	if (ImGui::BeginPopup("create_flyobject"))
	{
		ImGui::Text("Object Generator: ");
		ImGui::Separator();

		static char newObjectName[256] = "Name...";
		ImGui::InputText("Name", newObjectName, 256 * sizeof(char));

		if (ImGui::Button("Create")) {
			App->moduleWorldManager->GetSelectedRoom()->CreateFlyObject(newObjectName);
			ImGui::CloseCurrentPopup();
		}

		ImGui::EndPopup();
	}
}
