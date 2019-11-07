#include "imgui.h"
#include "Application.h"
#include "ModuleImGui.h"
#include "ModuleRoomManager.h"

#include "SceneDockPanel.h"

#include "Room.h"
#include "ViewportManager.h"
#include "FlyObject.h"

SceneDockPanel::SceneDockPanel(bool isVisible) : DockPanel("Scene", isVisible)
{
	flyEngineSection = FLY_SECTION_ROOM_EDIT;
	dockPanelType = DOCK_SCENE;
}

SceneDockPanel::~SceneDockPanel()
{
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
				ShowObjectCreatorTab();
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

void SceneDockPanel::ShowObjectCreatorTab()
{
	ImGui::PushFont(App->moduleImGui->headerFont);
	ImGui::Text("Object Generator: ");
	ImGui::PopFont();

	static char newObjectName[256] = "";
	ImGui::InputText("Name", newObjectName, 256 * sizeof(char));

	ImGui::Separator(); 
	ImGui::PushFont(App->moduleImGui->headerFont);
	ImGui::Text("Object Tools: ");
	ImGui::PopFont();

	ImGui::SameLine();
	
	if(ImGui::ImageButton(0, ImVec2(20, 20)))
	{
	
	}

	ImGui::BeginChild("##AttributesChild", ImVec2(ImGui::GetContentRegionAvailWidth(), 100), true);

	ImGui::EndChild();

	//static int attributeSelected = 0;
	//static int containsAttributeImage = false;

	//switch (attributeSelected)
	//{
	//	case 0:
	//		break;

	//	case 1:
	//		containsAttributeImage = true;
	//		break;
	//}

	//if (containsAttributeImage)
	//{
	//	if (ImGui::CollapsingHeader("Attribute Image Configuration"))
	//	{
	//		ImGui::BeginChild("AI_Child", ImVec2(ImGui::GetContentRegionAvailWidth(), 50), true);

	//		char buf[256] = "C:/";
	//		ImGui::InputText("##InputBrowse", buf, IM_ARRAYSIZE(buf));
	//		ImGui::SameLine();

	//		if (ImGui::Button("Browse")) {

	//		}

	//		ImGui::EndChild();
	//	}
	//}

	//ImGui::Combo("##NewAttributeCombo", &attributeSelected, "Select Attribute\0Image\0");
	//ImGui::SameLine();

	ImGui::PushFont(App->moduleImGui->headerFont);
	ImGui::SetCursorPosX(ImGui::GetContentRegionAvailWidth() - 50.0f); 
	if (ImGui::Button("Create"))
	{
		FlyObject* newObject = App->moduleRoomManager->GetSelectedRoom()->CreateFlyObject(newObjectName);

		//if (containsAttributeImage)
			//newObject->AddAttributeImage("Path");

		ImGui::CloseCurrentPopup();
	}
	ImGui::PopFont();
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

