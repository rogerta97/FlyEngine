#include "FlyObjectEditorUI.h"

#include "Application.h"
#include "ModuleImGui.h"
#include "Texture.h"
#include "ImageImporter.h"
#include "ResourceManager.h"
#include "ModuleRoomManager.h"
#include "Room.h"
#include "MyFileSystem.h"
#include "TinyFileDialog.h"
#include "FlyObject.h"

#include "imgui.h"

FlyObjectEditorUI* FlyObjectEditorUI::instance = 0;

FlyObjectEditorUI::FlyObjectEditorUI()
{

}

FlyObjectEditorUI* FlyObjectEditorUI::getInstance()
{
	if (instance == nullptr)
		instance = new FlyObjectEditorUI(); 

	return instance; 
}

FlyObjectEditorUI::~FlyObjectEditorUI()
{
	
}

void FlyObjectEditorUI::Delete()
{
	delete instance; 
}

//void FlyObjectEditorUI::Draw()
//{
//	// Name Bar --------
//	DrawToolsList();
//	DrawAddAndDeleteButtons();
//
//	switch (selectedInList)
//	{
//	case AT_IMAGE:
//
//		ImGui::Spacing();
//		ImGui::Separator();
//		ImGui::Spacing();
//
//		if (ImGui::CollapsingHeader("Image Tool Adjustments:"))
//		{
//			DrawToolProperties(selectedInList);
//		}
//
//		break;
//	}
//	
//	DrawPopups(); 
//
//	ImGui::Spacing();
//	ImGui::Separator();
//	ImGui::Spacing();
//
//	DrawObjectNameBar();
//
//	ImGui::SameLine(); 
//	ImGui::PushFont(App->moduleImGui->rudaBlackMid);
//	if (ImGui::Button("Create", ImVec2(80, 28)))
//	{
//		if (newObjectName == "") {
//			FLY_ERROR("Object with no name can not be created"); 
//			ImGui::PopFont();
//			return; 
//		}
//
//		FlyObject* newObject = App->moduleRoomManager->GetSelectedRoom()->CreateFlyObject(newObjectName);
//
//		if (showImageTool)
//			newObject->AddImageTool(previewImageTexture->GetPath());
//
//		CleanBooleans(); 
//
//		ImGui::CloseCurrentPopup();
//	}
//
//	ImGui::PopFont();
//}
//
//void FlyObjectEditorUI::DrawObjectNameBar()
//{
//	ImGui::PushFont(App->moduleImGui->rudaBoldMid);
//	ImGui::InputTextWithHint("##ObjectNaming", "Object Name...", newObjectName, 256 * sizeof(char));
//	ImGui::PopFont();
//}

void FlyObjectEditorUI::DrawToolsList(FlyObject* toolOwnerObject)
{
	ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0.12f, 0.14f, 0.17f, 1.00f)); 
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(5, 5));

	ImGui::BeginChild("##AttributesChild", ImVec2(ImGui::GetContentRegionAvailWidth(), 200));

	if (toolOwnerObject != nullptr)
	{
		for (auto& currentTool : toolOwnerObject->GetToolsList())
		{
			ToolSelectableInfo toolInfo = App->moduleManager->GetToolNameDescription(currentTool->GetToolName());
			//DrawToolSelectable(toolInfo, currentTool->isS); 
		}
	}

	ImGui::EndChild();

	ImGui::PopStyleVar();
	ImGui::PopStyleColor(); 
}

//void FlyObjectEditorUI::DrawAddAndDeleteButtons()
//{
//	ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0)); 
//	ImGui::PushStyleVar(ImGuiStyleVar_ItemInnerSpacing, ImVec2(2.0f, 2.0f)); 
//
//
//	Texture* plusIconTex = (Texture*)ResourceManager::getInstance()->GetResource("PlusIconWhite");
//	if (ImGui::ImageButton((ImTextureID)plusIconTex->GetTextureID(), ImVec2(18, 18)))
//	{
//		ImGui::OpenPopup("plusIconClicked");
//	}
//
//	ImGui::SameLine();
//	Texture* minusIconTex = (Texture*)ResourceManager::getInstance()->GetResource("MinusIconWhite");
//	if (ImGui::ImageButton((ImTextureID)minusIconTex->GetTextureID(), ImVec2(18, 18)))
//	{
//
//	}
//
//	ImGui::PopStyleVar();
//	ImGui::PopStyleColor(); 
//}

void FlyObjectEditorUI::DrawToolProperties(FlyObject* selectedObject, ToolType selectedObjectType)
{
	switch (selectedObjectType)
	{

	case AT_IMAGE:
		//DrawImageToolInfo();
		break;

	}
}

//void FlyObjectEditorUI::DrawImageToolInfo()
//{
//	static char buf[256] = "";
//
//	if (previewImageTexture == nullptr)
//		previewImageTexture = (Texture*)ResourceManager::getInstance()->GetResource("ImageNull");
//	else
//		previewImageTexture = (Texture*)ResourceManager::getInstance()->GetResource(previewImageTexture->GetName());
//
//	float aspect_ratio = previewImageTexture->GetTextureAspectRatio();
//	float previewQuadWidth = 150;
//	float previewQuadHeight = previewQuadWidth / aspect_ratio;
//
//	ImGui::Spacing();
//	PUSH_FONT(App->moduleImGui->rudaRegularMid);
//
//	ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0.12f, 0.14f, 0.17f, 1.00f));
//	ImGui::BeginChild("##4ShowImage", ImVec2(ImGui::GetContentRegionAvailWidth(), previewQuadHeight + 18));
//
//	ImGui::Columns(2);
//	ImGui::SetColumnWidth(0, previewQuadWidth + 10);
//
//	ImGui::Spacing();
//	ImGui::Image((ImTextureID)previewImageTexture->GetTextureID(), ImVec2(previewQuadWidth, previewQuadHeight));
//
//	ImGui::NextColumn();
//
//	ImGui::Spacing();
//	ImGui::Text("Name: "); ImGui::SameLine();
//	ImGui::TextColored(ImVec4(0.1f, 0.7f, 1.0f, 1.0f), "%s", previewImageTexture->GetName().c_str());
//
//	ImGui::Text("Width: "); ImGui::SameLine();
//	ImGui::TextColored(ImVec4(0.1f, 0.7f, 1.0f, 1.0f), "%d", previewImageTexture->GetWidth());
//
//	ImGui::Text("Height: "); ImGui::SameLine();
//	ImGui::TextColored(ImVec4(0.1f, 0.7f, 1.0f, 1.0f), "%d", previewImageTexture->GetHeight());
//
//	Texture* searchTexture = (Texture*)ResourceManager::getInstance()->GetResource("SearchIcon");
//	ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));
//
//	if (ImGui::ImageButton((ImTextureID)searchTexture->GetTextureID(), ImVec2(24, 24)))
//	{
//		char const* lFilterPatterns[2] = { "*.jpg" , "*.png" };
//		const char* path = tinyfd_openFileDialog("Load Image...", NULL, 2, lFilterPatterns, NULL, 0);
//
//		if (path != NULL)
//		{
//			if (!ResourceManager::getInstance()->ExistResourcePath(path))
//			{
//				previewImageTexture = ImageImporter::getInstance()->LoadTexture(path, false);
//				ResourceManager::getInstance()->AddResource(previewImageTexture, previewImageTexture->GetName());
//			}
//			else
//			{
//				previewImageTexture = (Texture*)ResourceManager::getInstance()->GetResourceByPath(path);
//			}
//
//			strcpy(buf, path);
//			FLY_LOG("Player Opened %s", path);
//		}
//
//	}
//
//	ImGui::PopStyleColor(2);
//	ImGui::EndChild();
//	ImGui::PopFont();
//}
//
//void FlyObjectEditorUI::DrawPopups()
//{
//	if (ImGui::BeginPopup("plusIconClicked"))
//	{
//		// Search Bar ---------------
//		ImGui::InputText("##SearchTool", searchToolBuffer, IM_ARRAYSIZE(searchToolBuffer));
//		ImGui::SameLine();
//
//		Texture* filterIcon = (Texture*)ResourceManager::getInstance()->GetResource("FilterIcon");
//		ImGui::Image((ImTextureID)filterIcon->GetTextureID(), ImVec2(22, 22));
//
//		ImGui::Spacing();
//		ImGui::Separator();
//		ImGui::Spacing();
//
//		// Tools Dictonary ----------
//		for (int i = 0; i < App->moduleManager->GetToolsAmount(); i++)
//		{
//			ToolSelectableInfo newToolInfo = App->moduleManager->GetToolNameDescription(i);
//
//			if (DrawToolSelectable(newToolInfo))
//			{
//				switch (newToolInfo.toolType)
//				{
//				case AT_IMAGE:
//					showImageTool = true;
//					break;
//
//				case AT_null:
//					break;
//				}
//			}
//		}
//
//		ImGui::EndPopup();
//	}
//}
//
//void FlyObjectEditorUI::CleanBooleans()
//{
//	if (showImageTool)
//	{
//		showImageTool = false; 
//		selectedInList = AT_null;
//		previewImageTexture = nullptr; 
//	}
//}
//
//void FlyObjectEditorUI::OnNewToolButtonClicked()
//{
//
//}

bool FlyObjectEditorUI::DrawToolSelectable(ToolSelectableInfo newToolInfo, bool isSelected)
{
	// Title ------
	bool clicked = false; 
	ImGui::PushFont(App->moduleImGui->rudaBoldMid);
	if (ImGui::Selectable(newToolInfo.toolName.c_str(), &isSelected, ImGuiSelectableFlags_None, ImVec2(ImGui::GetContentRegionAvailWidth(), 37))) {

		clicked = true; 
	}
	ImGui::PopFont();

	// Description -----
	ImGui::SetCursorPosY(ImGui::GetCursorPos().y - 20);
	ImGui::SetCursorPosX(ImGui::GetCursorPos().x + 2);

	ImGui::PushFont(App->moduleImGui->rudaRegularSmall);
	ImGui::TextWrapped(newToolInfo.toolDescription.c_str());
	ImGui::PopFont();


	return clicked; 

}
