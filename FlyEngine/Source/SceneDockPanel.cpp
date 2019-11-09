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

//void SceneDockPanel::ShowObjectCreatorTab()
//{
//	ImGui::Spacing(); 
//	ImGui::PushFont(App->moduleImGui->rudaBlackFont);
//	static char newObjectName[256] = "";
//	ImGui::InputText("Object Name", newObjectName, 256 * sizeof(char));
//	ImGui::PopFont();
//
//	ImGui::Spacing();
//	ImGui::Separator(); 
//	ImGui::Spacing();
//
//	DrawAddToolSection();
//
//	ImGui::Spacing(); 
//	ImGui::Separator();
//	ImGui::Spacing(); 
//
//	ImGui::SetCursorPosX(ImGui::GetContentRegionAvailWidth() - 80.0f); 
//
//	ImGui::PushFont(App->moduleImGui->rudaBlackFont);
//	if (ImGui::Button("Create", ImVec2(80, 30)))
//	{
//		//FlyObject* newObject = App->moduleRoomManager->GetSelectedRoom()->CreateFlyObject(newObjectName);
//
//		//if (containsAttributeImage)
//			//newObject->AddAttributeImage("Path");
//
//		ImGui::CloseCurrentPopup();
//	}
//
//	ImGui::PopFont(); 
////	ImGui::PopFont();
//}
//
//void SceneDockPanel::DrawAddToolSection()
//{
//	ImGui::PushFont(App->moduleImGui->rudaBoldFont);
//	ImGui::Text("Object Tools: ");
//	ImGui::PopFont();
//
//	ImGui::BeginChild("##AttributesChild", ImVec2(ImGui::GetContentRegionAvailWidth(), 200), true);
//
//	ImGui::Selectable("Example");
//	ImGui::Text("Hello");
//
//	ImGui::EndChild();
//
//	Texture* plusIconTex = (Texture*)ResourceManager::getInstance()->GetResource("PlusIcon");
//	if (ImGui::ImageButton((ImTextureID)plusIconTex->GetTextureID(), ImVec2(18, 18)))
//	{
//		ImGui::OpenPopup("plusIconClicked");
//	}
//
//	ImGui::SameLine();
//	Texture* minusIconTex = (Texture*)ResourceManager::getInstance()->GetResource("MinusIcon");
//	if (ImGui::ImageButton((ImTextureID)minusIconTex->GetTextureID(), ImVec2(18, 18)))
//	{
//
//	}
//
//	DrawAddToolPopup();
//}
//
//void SceneDockPanel::DrawAddToolPopup()
//{
//	if (ImGui::BeginPopup("plusIconClicked"))
//	{
//		// Search Bar ---------------
//		ImGui::InputText("##SearchTool", searchToolBuffer, IM_ARRAYSIZE(searchToolBuffer));
//		ImGui::SameLine();
//		
//		Texture* searchIcon = (Texture*)ResourceManager::getInstance()->GetResource("SearchIcon");
//		if (ImGui::ImageButton((ImTextureID)searchIcon->GetTextureID(), ImVec2(18, 18))) {
//		
//		}
//		
//		ImGui::Spacing();
//		ImGui::Separator();
//		ImGui::Spacing(); 
//
//		// Tools Dictonary ----------
//		for (int i = 0; i < App->moduleManager->GetToolsAmount(); i++) 
//		{
//			ToolSelectableInfo newToolInfo = App->moduleManager->GetToolNameDescription(i);
//			DrawSearchToolSelectable(newToolInfo.toolName.c_str(), newToolInfo.toolDescription.c_str(), newToolInfo.toolType);
//		}
//		
//		ImGui::EndPopup();
//	}
//}
//
//void SceneDockPanel::DrawSearchToolSelectable(const char* toolName = "", const char* toolDescription = "", ToolType toolType = AT_null)
//{
//
//}

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

