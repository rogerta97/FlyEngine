#include "ObjectCreatorDockPanel.h"

#include "Application.h"
#include "TinyFileDialog.h"
#include "ModuleImGui.h"
#include "Tool.h"
#include "ModuleRoomManager.h"
#include "Room.h"
#include "Texture.h"
#include "ImageImporter.h"
#include "ImageTool.h"
#include "ResourceManager.h"

#include "FlyObject.h"
#include "imgui.h"

#include "mmgr.h"

ObjectCreatorDockPanel::ObjectCreatorDockPanel(bool isVisible) : DockPanel("Entity Creator", isVisible)
{
	flyEngineSection = FLY_SECTION_null;
	dockPanelType = DOCK_OBJECT_CREATOR;
	ResetObjectData(); 
}

ObjectCreatorDockPanel::~ObjectCreatorDockPanel()
{

}

bool ObjectCreatorDockPanel::Draw()
{
#pragma region secutiryChecks
	if (!DockPanel::Draw())
		return false;
#pragma endregion

	if (ImGui::Begin(panelName.c_str(), &visible))
	{
		DrawObjectCreator(); 
	}

	ImGui::End(); 
}

void ObjectCreatorDockPanel::ResetObjectData()
{
	strcpy(newObjectName, ""); 
	strcpy(searchNewToolBuffer, ""); 
	selectedTool = nullptr; 
	creatingObject = new FlyObject("Prev");
}

void ObjectCreatorDockPanel::DrawObjectCreator()
{
	DrawObjectToolsList(); 
	DrawAddAndDeleteToolButtons(); 
	//DrawSelectedToolSettings(); 
	DrawCreateButton();
}

void ObjectCreatorDockPanel::DrawObjectToolsList()
{
	ImGui::PushFont(App->moduleImGui->rudaBoldBig);
	ImGui::Text("Add Object Actions: ");
	ImGui::PopFont();

	ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0.12f, 0.14f, 0.17f, 1.00f));
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(5, 5));

	ImGui::BeginChild("##AttributesChild", ImVec2(ImGui::GetContentRegionAvailWidth(), 200));

	for (auto& currentTool : creatingObject->GetToolsList())
	{
		ToolSelectableInfo selectableInfo = currentTool->GetToolSelectableInfo();
		DrawSelectable(selectableInfo, currentTool->isSelected, currentTool);
	}

	ImGui::EndChild();

	ImGui::PopStyleVar();
	ImGui::PopStyleColor();
}

void ObjectCreatorDockPanel::DrawSelectable(ToolSelectableInfo selectableInfo, bool& isSelected, Tool* currentTool = nullptr)
{
	ImGui::PushFont(App->moduleImGui->rudaBoldMid);

	if (ImGui::Selectable(selectableInfo.toolName.c_str(), &isSelected, ImGuiSelectableFlags_None, ImVec2(ImGui::GetContentRegionAvailWidth(), 37))) {
		creatingObject->selectedTool = currentTool;
		selectedTool = currentTool; 
	}
	ImGui::PopFont();

	// Description -----
	ImGui::SetCursorPosY(ImGui::GetCursorPos().y - 20);
	ImGui::SetCursorPosX(ImGui::GetCursorPos().x + 2);

	ImGui::PushFont(App->moduleImGui->rudaRegularSmall);
	ImGui::TextWrapped(selectableInfo.toolDescription.c_str());
	ImGui::PopFont();
}

void ObjectCreatorDockPanel::DrawSelectedToolSettings()
{
	if (selectedTool)
	{
		if (selectedTool->GetToolType() != AT_null) {
			IMGUI_SPACED_SEPARATOR
		}

		switch (selectedTool->GetToolType())
		{
		case AT_IMAGE:
			DrawToolImageSettings();
			break;

		case AT_CHANGE_SCENE:
			break;
		}

	}
}

void ObjectCreatorDockPanel::OnAddToolButtonClicked()
{
	if (ImGui::BeginPopup("plusIconClicked"))
	{
		// Search Bar ---------------
		ImGui::InputText("##SearchTool", searchNewToolBuffer, IM_ARRAYSIZE(searchNewToolBuffer));
		ImGui::SameLine();

		Texture* filterIcon = (Texture*)ResourceManager::getInstance()->GetResource("FilterIcon");
		ImGui::Image((ImTextureID)filterIcon->GetTextureID(), ImVec2(22, 22));

		ImGui::Spacing();
		ImGui::Separator();
		ImGui::Spacing();

		// Tools Dictonary ----------
		ToolSelectableInfo* newToolSelected = App->moduleManager->DrawToolDictionaryUI(); 
		if (newToolSelected != nullptr)
		{
			switch (newToolSelected->toolType)
			{
			case AT_IMAGE:
				creatingObject->AddImageTool("None"); 
				break;

			case AT_null:
				break;
			}
		}

		ImGui::EndPopup();
	}
}

void ObjectCreatorDockPanel::Close() 
{
	selectedTool = nullptr;
	visible = false; 
}

void ObjectCreatorDockPanel::DrawCreateButton()
{
	ImGui::SetCursorPosY(ImGui::GetContentRegionMax().y - 35); 

	ImGui::PushFont(App->moduleImGui->rudaBoldMid);
	ImGui::InputTextWithHint("##ObjectNaming", "Object Name...", newObjectName, 256 * sizeof(char));
	ImGui::PopFont();

	ImGui::PushFont(App->moduleImGui->rudaBlackMid);
	ImGui::SameLine();
	if (ImGui::Button("Create", ImVec2(80, 28)))
	{
		std::string newObjectNameStr(newObjectName);
		if (newObjectNameStr.empty()) {
			FLY_ERROR("Object with no name can not be created");
			ImGui::PopFont();
			return;
		}
		else 
		{
			creatingObject->SetName(newObjectName); 
			App->moduleRoomManager->GetSelectedRoom()->AddFlyObject(creatingObject);				
		}

		Close();
	}

	ImGui::PopFont();
}

void ObjectCreatorDockPanel::DrawToolImageSettings()
{
	if (ImGui::CollapsingHeader("Image Adjustments"))
	{
		static char buf[256] = "";
		ImageTool* selectedImageTool = (ImageTool*)this->selectedTool; 

		if (selectedImageTool->GetTexture() == nullptr)
			selectedImageTool->SetTexture((Texture*)ResourceManager::getInstance()->GetResource("ImageNull"));
		else
			selectedImageTool->SetTexture((Texture*)ResourceManager::getInstance()->GetResource(selectedImageTool->GetTexture()->GetName()));

		float aspect_ratio = selectedImageTool->GetTexture()->GetAspectRatio();
		float previewQuadWidth = 150;
		float previewQuadHeight = previewQuadWidth / aspect_ratio;

		ImGui::Spacing();
		PUSH_FONT(App->moduleImGui->rudaRegularMid);

		ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0.12f, 0.14f, 0.17f, 1.00f));

		int childHeight = previewQuadHeight + 20; 
		ImGui::BeginChild("##4ShowImage", ImVec2(ImGui::GetContentRegionAvailWidth(), childHeight));

		ImGui::Columns(2);
		ImGui::SetColumnWidth(0, previewQuadWidth + 10);

		ImGui::Spacing();
		ImGui::Image((ImTextureID)selectedImageTool->GetTexture()->GetTextureID(), ImVec2(previewQuadWidth, previewQuadHeight));

		ImGui::NextColumn();

		ImGui::Spacing();
		ImGui::Text("Name: "); ImGui::SameLine();
		ImGui::TextColored(ImVec4(0.1f, 0.7f, 1.0f, 1.0f), "%s", selectedImageTool->GetTexture()->GetName().c_str());

		ImGui::Text("Width: "); ImGui::SameLine();
		ImGui::TextColored(ImVec4(0.1f, 0.7f, 1.0f, 1.0f), "%d", selectedImageTool->GetTexture()->GetWidth());

		ImGui::Text("Height: "); ImGui::SameLine();
		ImGui::TextColored(ImVec4(0.1f, 0.7f, 1.0f, 1.0f), "%d", selectedImageTool->GetTexture()->GetHeight());

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
					selectedImageTool->SetTexture(ImageImporter::getInstance()->LoadTexture(path, false));
					ResourceManager::getInstance()->AddResource(selectedImageTool->GetTexture(), selectedImageTool->GetTexture()->GetName());
				}
				else
				{
					selectedImageTool->SetTexture((Texture*)ResourceManager::getInstance()->GetResourceByPath(path));
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

void ObjectCreatorDockPanel::DrawAddAndDeleteToolButtons()
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
		if (selectedTool != nullptr) {
			creatingObject->DeleteTool(selectedTool->GetToolName()); 
		}
	}

	ImGui::PopStyleVar();
	ImGui::PopStyleColor();

	// Callbacks for buttons 
	OnAddToolButtonClicked(); 
}