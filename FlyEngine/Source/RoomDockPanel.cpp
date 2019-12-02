#include "imgui.h"
#include "Application.h"
#include "ModuleImGui.h"
#include "ModuleRoomManager.h"
#include "ModuleManager.h"
#include "ResourceManager.h"

#include "RoomDockPanel.h"
#include "ImageImporter.h"
#include "TinyFileDialog.h"

#include "Room.h"
#include "Texture.h"
#include "ViewportManager.h"
#include "FlyObject.h"
#include "mmgr.h"

RoomDockPanel::RoomDockPanel(bool isVisible) : DockPanel("Room", isVisible)
{
	flyEngineSection = FLY_SECTION_ROOM_EDIT;
	dockPanelType = DOCK_SCENE;
}

RoomDockPanel::~RoomDockPanel()
{

}

bool RoomDockPanel::Draw()
{

#pragma region secutiryChecks
	if (!DockPanel::Draw())
		return false;
#pragma endregion

	if (ImGui::Begin(panelName.c_str(), &visible))
	{
		ImGui::ImageButton(0, ImVec2(30, 30)); 

		ImGui::SameLine();
		ImGui::ImageButton(0, ImVec2(30, 30));
		

		ImGuiTabBarFlags tab_bar_flags = ImGuiTabBarFlags_None;
		if (ImGui::BeginTabBar("MyTabBar", tab_bar_flags))
		{
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

void RoomDockPanel::ShowViewportSettingsTab()
{
	ImGui::Spacing(); 

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

