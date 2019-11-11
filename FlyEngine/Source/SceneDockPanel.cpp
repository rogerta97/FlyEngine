#include "imgui.h"
#include "Application.h"
#include "ModuleImGui.h"
#include "ModuleRoomManager.h"
#include "ModuleManager.h"
#include "ResourceManager.h"

#include "SceneDockPanel.h"
#include "ImageImporter.h"
#include "TinyFileDialog.h"

#include "Room.h"
#include "Texture.h"
#include "ViewportManager.h"
#include "FlyObject.h"
#include "mmgr.h"

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
				if (ImGui::IsItemClicked(1)) {
					FLY_LOG("Reset Object Data"); 
				}
			
				DrawObjectCreator();

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

void SceneDockPanel::DrawObjectCreator()
{
	DrawCreatorToolsList();
	DrawCreatorAddAndDeleteButtons();
	DrawSelectedToolSettings();
	DrawCreatorBottomBar();

	OnAddButtonClicked();
}

void SceneDockPanel::DrawSelectedToolSettings()
{
	if (toolAdjustmentsOwnerType != AT_null) {
		IMGUI_SPACED_SEPARATOR
	}

	switch (toolAdjustmentsOwnerType)
	{
	case AT_IMAGE:
		DrawToolImageSettings();
		break;

	case AT_CHANGE_SCENE:
		break;
	}

	IMGUI_SPACED_SEPARATOR
}

void SceneDockPanel::DrawToolImageSettings()
{
	if (ImGui::CollapsingHeader("Image Adjustments"))
	{
		static char buf[256] = "";

		if (previewImageTexture == nullptr)
			previewImageTexture = (Texture*)ResourceManager::getInstance()->GetResource("ImageNull");
		else
			previewImageTexture = (Texture*)ResourceManager::getInstance()->GetResource(previewImageTexture->GetName());

		float aspect_ratio = previewImageTexture->GetTextureAspectRatio();
		float previewQuadWidth = 150;
		float previewQuadHeight = previewQuadWidth / aspect_ratio;

		ImGui::Spacing();
		PUSH_FONT(App->moduleImGui->rudaRegularMid);

		ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0.12f, 0.14f, 0.17f, 1.00f));
		ImGui::BeginChild("##4ShowImage", ImVec2(ImGui::GetContentRegionAvailWidth(), previewQuadHeight + 18));

		ImGui::Columns(2);
		ImGui::SetColumnWidth(0, previewQuadWidth + 10);

		ImGui::Spacing();
		ImGui::Image((ImTextureID)previewImageTexture->GetTextureID(), ImVec2(previewQuadWidth, previewQuadHeight));

		ImGui::NextColumn();

		ImGui::Spacing();
		ImGui::Text("Name: "); ImGui::SameLine();
		ImGui::TextColored(ImVec4(0.1f, 0.7f, 1.0f, 1.0f), "%s", previewImageTexture->GetName().c_str());

		ImGui::Text("Width: "); ImGui::SameLine();
		ImGui::TextColored(ImVec4(0.1f, 0.7f, 1.0f, 1.0f), "%d", previewImageTexture->GetWidth());

		ImGui::Text("Height: "); ImGui::SameLine();
		ImGui::TextColored(ImVec4(0.1f, 0.7f, 1.0f, 1.0f), "%d", previewImageTexture->GetHeight());

		Texture* searchTexture = (Texture*)ResourceManager::getInstance()->GetResource("SearchIcon");
		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));

		if (ImGui::ImageButton((ImTextureID)searchTexture->GetTextureID(), ImVec2(24, 24)))
		{
			char const* lFilterPatterns[2] = { "*.jpg" , "*.png" };
			const char* path = tinyfd_openFileDialog("Load Image...", NULL, 2, lFilterPatterns, NULL, 0);

			if (path != NULL)
			{
				if (!ResourceManager::getInstance()->ExistResourcePath(path))
				{
					previewImageTexture = ImageImporter::getInstance()->LoadTexture(path, false);
					ResourceManager::getInstance()->AddResource(previewImageTexture, previewImageTexture->GetName());
				}
				else
				{
					previewImageTexture = (Texture*)ResourceManager::getInstance()->GetResourceByPath(path);
				}

				strcpy(buf, path);
				FLY_LOG("Player Opened %s", path);
			}

		}

		ImGui::PopStyleColor(2);
		ImGui::EndChild();
		ImGui::PopFont();
	}
}

void SceneDockPanel::DrawCreatorBottomBar()
{
	ImGui::PushFont(App->moduleImGui->rudaBoldMid);
	ImGui::InputTextWithHint("##ObjectNaming", "Object Name...", newObjectName, 256 * sizeof(char));
	ImGui::PopFont();

	ImGui::PushFont(App->moduleImGui->rudaBlackMid);
	ImGui::SameLine(); 
	if (ImGui::Button("Create", ImVec2(80, 28)))
	{
		if (newObjectName == "") {
			FLY_ERROR("Object with no name can not be created");
			ImGui::PopFont();
			return;
		}

		FlyObject* newObject = App->moduleRoomManager->GetSelectedRoom()->CreateFlyObject(newObjectName);

		if (showImageTool)
			newObject->AddImageTool(previewImageTexture->GetPath());

		ImGui::CloseCurrentPopup();
	}

	ImGui::PopFont();
}

void SceneDockPanel::OnAddButtonClicked()
{
	if (ImGui::BeginPopup("plusIconClicked"))
	{
		// Search Bar ---------------
		ImGui::InputText("##SearchTool", searchToolBuffer, IM_ARRAYSIZE(searchToolBuffer));
		ImGui::SameLine();

		Texture* filterIcon = (Texture*)ResourceManager::getInstance()->GetResource("FilterIcon");
		ImGui::Image((ImTextureID)filterIcon->GetTextureID(), ImVec2(22, 22));

		ImGui::Spacing();
		ImGui::Separator();
		ImGui::Spacing();

		// Tools Dictonary ----------
		for (int i = 0; i < App->moduleManager->GetToolsAmount(); i++)
		{
			ToolSelectableInfo newToolInfo = App->moduleManager->GetToolNameDescription(i);

			if (DrawToolSelectable(newToolInfo))
			{
				switch (newToolInfo.toolType)
				{
				case AT_IMAGE:
					showImageTool = true;
					break;

				case AT_null:
					break;
				}
			}
		}

		ImGui::EndPopup();
	}
}

void SceneDockPanel::DrawCreatorAddAndDeleteButtons()
{
	ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));
	ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(2, 2));

	ImGui::SetCursorPosY(ImGui::GetCursorPosY() - 5); 
	Texture* plusIconTex = (Texture*)ResourceManager::getInstance()->GetResource("PlusIconWhite");
	if (ImGui::ImageButton((ImTextureID)plusIconTex->GetTextureID(), ImVec2(18, 18)))
	{
		ImGui::OpenPopup("plusIconClicked");
	}

	ImGui::SameLine();
	Texture* minusIconTex = (Texture*)ResourceManager::getInstance()->GetResource("MinusIconWhite");
	if (ImGui::ImageButton((ImTextureID)minusIconTex->GetTextureID(), ImVec2(18, 18)))
	{

	}

	ImGui::PopStyleVar();
	ImGui::PopStyleColor();
}

void SceneDockPanel::DrawCreatorToolsList()
{
	ImGui::PushFont(App->moduleImGui->rudaBoldBig);
	ImGui::Text("Add Object Actions: ");
	ImGui::PopFont();

	ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0.12f, 0.14f, 0.17f, 1.00f));
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(5, 5));

	ImGui::BeginChild("##AttributesChild", ImVec2(ImGui::GetContentRegionAvailWidth(), 200));

	if (showImageTool)
	{
		ToolSelectableInfo toolInfo = App->moduleManager->GetToolNameDescription("Image");

		if (DrawToolSelectable(toolInfo))
			toolAdjustmentsOwnerType = toolInfo.toolType; 
	}

	ImGui::EndChild();

	ImGui::PopStyleVar();
	ImGui::PopStyleColor();
}

bool SceneDockPanel::DrawToolSelectable(ToolSelectableInfo& toolInfo)
{
	bool clicked = false; 
	ImGui::PushFont(App->moduleImGui->rudaBoldMid);
	if (ImGui::Selectable(toolInfo.toolName.c_str(), true, ImGuiSelectableFlags_None, ImVec2(ImGui::GetContentRegionAvailWidth(), 37))) {
		clicked = true; 
	}
	ImGui::PopFont();

	// Description -----
	ImGui::SetCursorPosY(ImGui::GetCursorPos().y - 20);
	ImGui::SetCursorPosX(ImGui::GetCursorPos().x + 2);

	ImGui::PushFont(App->moduleImGui->rudaRegularSmall);
	ImGui::TextWrapped(toolInfo.toolDescription.c_str());
	ImGui::PopFont();

	return clicked; 
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

