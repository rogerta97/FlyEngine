#include "ObjectPropertiesDockPanel.h"
#include "Application.h"
#include "ModuleImGui.h"
#include "ModuleManager.h"
#include "imgui.h"
#include "Tool.h"
#include "ModuleRoomManager.h"
#include "Gizmos.h"
#include "ResourceManager.h"
#include "ImageTool.h"
#include "ImageImporter.h"
#include "TinyFileDialog.h"
#include "Texture.h"
#include "FlyObject.h"
#include "Room.h"
#include "mmgr.h"


ObjectPropertiesDockPanel::ObjectPropertiesDockPanel(bool isVisible) : DockPanel("Object Properties", isVisible)
{
	flyEngineSection = FLY_SECTION_ROOM_EDIT;
	dockPanelType = DOCK_OBJECT_PROPERTIES;

}

ObjectPropertiesDockPanel::~ObjectPropertiesDockPanel()
{

}

bool ObjectPropertiesDockPanel::Draw()
{
#pragma region secutiryChecks
	if (!DockPanel::Draw())
		return false;
#pragma endregion

	if (ImGui::Begin(panelName.c_str(), &visible)) 
	{
		FlyObject* selectedObject = App->moduleRoomManager->GetSelectedRoom()->GetSelectedObject();

		if (selectedObject != nullptr)
		{
			Texture* objectIconTextue = (Texture*)ResourceManager::getInstance()->GetResource("ObjectIcon");
			ImGui::Image((ImTextureID)objectIconTextue->GetTextureID(), ImVec2(35, 35));

			ImGui::Separator();

			ImGui::SetCursorPos(ImVec2(ImGui::GetCursorPosX() + 45, ImGui::GetCursorPosY() - 35));

			ImGui::PushFont(App->moduleImGui->rudaBoldHuge);
			ImGui::Text("%s", selectedObject->GetName().c_str());
			ImGui::PopFont();

			ImGui::Spacing();

			//DrawObjectPlacementCH();

			ImGui::Spacing();
			ImGui::Separator(); 
			ImGui::Spacing();

			DrawObjectTools();
		}
		else
		{
			ImGui::TextColored(ImVec4(0.2f, 0.2f, 0.2f, 1.0f), "No Object Selected");
		}
	}

	ImGui::End();
	return true;
}

void ObjectPropertiesDockPanel::DrawObjectTools()
{
	// Draw Objects List ---------
	DrawToolList();

	ImGui::Spacing();
	ImGui::Separator();
	ImGui::Spacing(); 

	// Draw Tool --------
	DrawToolAdjustments();
}

void ObjectPropertiesDockPanel::DrawToolAdjustments()
{
	if (selectedObject->selectedTool)
	{
		switch (selectedObject->selectedTool->GetToolType())
		{
		case AT_IMAGE:
			DrawToolImageSettings(); 
			break; 
		}
	}
}

void ObjectPropertiesDockPanel::DrawToolList()
{
	ImGui::PushFont(App->moduleImGui->rudaBoldBig);
	ImGui::Text("Object Actions: ");
	ImGui::PopFont();

	ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0.12f, 0.14f, 0.17f, 1.00f));
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(5, 5));

	ImGui::BeginChild("##ToolsListObjectProperties", ImVec2(ImGui::GetContentRegionAvailWidth(), 200));

	for (auto& currentTool : selectedObject->GetToolsList()) 
	{
		ToolSelectableInfo selectableInfo = currentTool->GetToolSelectableInfo(); 
		DrawToolSelectable(selectableInfo, currentTool);
	}

	ImGui::EndChild();

	ImGui::PopStyleVar();
	ImGui::PopStyleColor();

	DrawAddAndDeleteButtons();
}

void ObjectPropertiesDockPanel::DrawAddAndDeleteButtons()
{
	ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));
	ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(2, 2));

	ImGui::SetCursorPosY(ImGui::GetCursorPosY() - 5);
	Texture* plusIconTex = (Texture*)ResourceManager::getInstance()->GetResource("PlusIconWhite");
	if (ImGui::ImageButton((ImTextureID)plusIconTex->GetTextureID(), ImVec2(18, 18)))
	{
		ImGui::OpenPopup("add_tool_from_object_properties"); 

	}

	ImGui::SameLine();
	Texture* minusIconTex = (Texture*)ResourceManager::getInstance()->GetResource("MinusIconWhite");
	if (ImGui::ImageButton((ImTextureID)minusIconTex->GetTextureID(), ImVec2(18, 18)))
	{	
		Tool* selectedTool = selectedObject->selectedTool; 

		if (selectedTool != nullptr)
		{
			selectedTool->CleanUp();
			selectedObject->DeleteTool(selectedTool->GetToolName());
			selectedTool = nullptr;
		}
	}

	ImGui::PopStyleVar();
	ImGui::PopStyleColor();	

	if (ImGui::BeginPopup("add_tool_from_object_properties"))
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
				selectedObject->AddImageTool("None");
				break;

			case AT_null:
				break;
			}
		}

		ImGui::EndPopup();
	}
}

void ObjectPropertiesDockPanel::DrawToolSelectable(ToolSelectableInfo& selectableInfo, Tool*& currentTool)
{
	ImGui::PushFont(App->moduleImGui->rudaBoldMid);
	if (ImGui::Selectable(selectableInfo.toolName.c_str(), &currentTool->isSelected, ImGuiSelectableFlags_None, ImVec2(ImGui::GetContentRegionAvailWidth(), 37))) {
		selectedObject->selectedTool = currentTool;
	}
	ImGui::PopFont();

	// Description -----
	ImGui::SetCursorPosY(ImGui::GetCursorPos().y - 20);
	ImGui::SetCursorPosX(ImGui::GetCursorPos().x + 2);

	ImGui::PushFont(App->moduleImGui->rudaRegularSmall);
	ImGui::TextWrapped(selectableInfo.toolDescription.c_str());
	ImGui::PopFont();
}

void ObjectPropertiesDockPanel::DrawObjectPlacementCH()
{
	ImGui::PushFont(App->moduleImGui->rudaBoldMid);
	if (ImGui::CollapsingHeader("Object Placement"))
	{
		ImGui::Spacing(); 

		float showPosition[2] = { selectedObject->transform->GetPosition().x, selectedObject->transform->GetPosition().y };
		float showRotation[2] = { selectedObject->transform->GetRotation().x, selectedObject->transform->GetRotation().y };
		float showScale[2] = { selectedObject->transform->GetScale().x, selectedObject->transform->GetScale().y };

		PUSH_FONT(App->moduleImGui->rudaRegularMid);
		if (ImGui::DragFloat2("Position", showPosition, 0.5f))
		{
			selectedObject->transform->SetPosition(float2(showPosition[0], showPosition[1]));
			selectedObject->gizmos->CalculateGizmos(); 
		}

		if (ImGui::DragFloat2("Rotation", showRotation))
		{
			selectedObject->transform->SetRotationEuler(float2(showRotation[0], showRotation[1]));
		}

		if (ImGui::DragFloat2("Scale", showScale, 0.1f))
		{
			selectedObject->transform->SetScale(float2(showScale[0], showScale[1]));
			selectedObject->gizmos->CalculateGizmos();
		}
		POP_FONT;
	}
	POP_FONT;
}

void ObjectPropertiesDockPanel::DrawToolImageSettings()
{
	ImageTool* imageTool = (ImageTool*)selectedObject->GetTool("Image");

	if (imageTool != nullptr)
	{
		if (ImGui::CollapsingHeader("Image Tool Settings"))
		{
			static char buf[256] = "";

			Texture* imageToolTexture = imageTool->GetTexture(); 

			if (imageToolTexture == nullptr)
				imageToolTexture = (Texture*)ResourceManager::getInstance()->GetResource("ImageNull");

			float aspect_ratio = imageToolTexture->GetAspectRatio();
			float previewQuadWidth = 150;
			float previewQuadHeight = previewQuadWidth / aspect_ratio;

			ImGui::Spacing();
			PUSH_FONT(App->moduleImGui->rudaRegularMid);

			ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0.12f, 0.14f, 0.17f, 1.00f));
			ImGui::BeginChild("##4ShowImage", ImVec2(ImGui::GetContentRegionAvailWidth(), previewQuadHeight + 18));

			ImGui::Columns(2);
			ImGui::SetColumnWidth(0, previewQuadWidth + 10);

			ImGui::Spacing();
			ImGui::Image((ImTextureID)imageToolTexture->GetTextureID(), ImVec2(previewQuadWidth, previewQuadHeight));

			ImGui::NextColumn();

			ImGui::Spacing();
			ImGui::Text("Name: "); ImGui::SameLine();
			ImGui::TextColored(ImVec4(0.1f, 0.7f, 1.0f, 1.0f), "%s", imageToolTexture->GetName().c_str());

			ImGui::Text("Width: "); ImGui::SameLine();
			ImGui::TextColored(ImVec4(0.1f, 0.7f, 1.0f, 1.0f), "%d", imageToolTexture->GetWidth());

			ImGui::Text("Height: "); ImGui::SameLine();
			ImGui::TextColored(ImVec4(0.1f, 0.7f, 1.0f, 1.0f), "%d", imageToolTexture->GetHeight());

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
						imageToolTexture = ImageImporter::getInstance()->LoadTexture(path, false);
						ResourceManager::getInstance()->AddResource(imageToolTexture, imageToolTexture->GetName());
					}
					else
					{
						imageToolTexture = (Texture*)ResourceManager::getInstance()->GetResourceByPath(path);
					}

					imageTool->SetTexture(imageToolTexture); 

					strcpy(buf, path);
					FLY_LOG("Player Opened %s", path);
				}

			}

			ImGui::PopStyleColor(2);
			ImGui::EndChild();
			ImGui::PopFont();
		}
	}
}

void ObjectPropertiesDockPanel::SetSelectedObject(FlyObject* newSelectedObject)
{
	selectedObject = newSelectedObject; 
}

FlyObject* ObjectPropertiesDockPanel::GetSelectedObject() const
{
	return selectedObject;
}
