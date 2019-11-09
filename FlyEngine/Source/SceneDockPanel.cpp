#include "imgui.h"
#include "Application.h"
#include "ModuleImGui.h"
#include "ModuleRoomManager.h"
#include "ModuleManager.h"
#include "ResourceManager.h"

#include "SceneDockPanel.h"
#include "ObjectCreator.h"

#include "Room.h"
#include "Texture.h"
#include "ViewportManager.h"
#include "FlyObject.h"

SceneDockPanel::SceneDockPanel(bool isVisible) : DockPanel("Scene", isVisible)
{
	flyEngineSection = FLY_SECTION_ROOM_EDIT;
	dockPanelType = DOCK_SCENE;

	objectCreator = new ObjectCreator(); 
}

SceneDockPanel::~SceneDockPanel()
{
	delete objectCreator; 
}

bool SceneDockPanel::Draw()
{

#pragma region secutiryChecks
	if (!DockPanel::Draw())
		return false;
#pragma endregion

	if (ImGui::Begin(panelName.c_str(), &visible))
	{
		ImGuiTabBarFlags tab_bar_flags = ImGuiTabBarFlags_None;
		if (ImGui::BeginTabBar("MyTabBar", tab_bar_flags))
		{
			if (ImGui::BeginTabItem("Object Creator"))
			{
				if (ImGui::IsItemClicked(1)) {
					FLY_LOG("Reset Object Data"); 
				}

				objectCreator->Draw(); 
				ImGui::EndTabItem();
			}

			if (ImGui::BeginTabItem("Viewport Settings")) 
			{
				ShowViewportSettingsTab();
				ImGui::EndTabItem();
			}

			ImGui::EndTabBar();
		}
	}

	ImGui::End();
	return true;
}

void SceneDockPanel::ShowViewportSettingsTab()
{
	static int resolutionSelected = 0;
	if (ImGui::Combo("Resolution", &resolutionSelected, "4:3\0 1:1\0")) {

		switch (resolutionSelected)
		{

		case 0:
			ViewportManager::getInstance()->SetAspectRatioType(AR_4_3);
			break;

		case 1:
		{
			ViewportManager::getInstance()->SetAspectRatioType(AR_1_1);
			break;
		}

		default:
			break;
		}
	}
}

