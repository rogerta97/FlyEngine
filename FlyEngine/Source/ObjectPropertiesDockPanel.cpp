#include "ObjectPropertiesDockPanel.h"
#include "Application.h"
#include "ModuleImGui.h"
#include "ModuleManager.h"
#include "imgui.h"
#include "Action.h"
#include "ModuleRoomManager.h"
#include "Gizmos.h"
#include "ResourceManager.h"
#include "DisplayImageAction.h"
#include "GameViewportDockPanel.h"
#include "ImageImporter.h"
#include "ViewportManager.h"
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
			DrawFixedPartObjectUI(selectedObject);

			if (ImGui::BeginTabBar("MyTabBar", ImGuiTabBarFlags_None))
			{
				if (ImGui::BeginTabItem("Actions"))
				{
					DrawObjectActionsTab();
					ImGui::EndTabItem();
				}
				if (ImGui::BeginTabItem("Clickable Area"))
				{
					DrawClickableAreaTab(); 
					ImGui::EndTabItem();
				}
				ImGui::EndTabBar();
			}		
		}
		else
		{
			ImGui::TextColored(ImVec4(0.8f, 0.5f, 0.5f, 1.0f), "No Object Selected");
		}
	}

	ImGui::End();
	return true;
}

void ObjectPropertiesDockPanel::DrawFixedPartObjectUI(FlyObject* selectedObject)
{
	Texture* objectIconTextue = (Texture*)ResourceManager::getInstance()->GetResource("ObjectIcon");
	ImGui::Image((ImTextureID)objectIconTextue->GetTextureID(), ImVec2(35, 35));

	ImGui::Separator();

	ImGui::SetCursorPos(ImVec2(ImGui::GetCursorPosX() + 45, ImGui::GetCursorPosY() - 35));

	ImGui::PushFont(App->moduleImGui->rudaBoldHuge);
	ImGui::Text("%s", selectedObject->GetName().c_str());
	ImGui::PopFont();

	if (!selectedObject->GetDescription().empty()) {
		ImGui::Spacing();	
		ImGui::TextColored(ImVec4(0.8f, 0.8f, 1.0f, 1.0f), "%s", selectedObject->GetDescription().c_str());
	}

	ImGui::Spacing();
	DrawObjectPlacementCH();
	ImGui::Spacing();
}

void ObjectPropertiesDockPanel::DrawObjectActionsTab()
{
	// Draw Objects List ---------
	DrawActionsList();

	ImGui::Spacing();
	ImGui::Separator();
	ImGui::Spacing(); 

	// Draw Tool --------
	DrawActionSettings();
}

void ObjectPropertiesDockPanel::DrawClickableAreaTab()
{
	ImGui::Spacing();

	ImGui::Checkbox("Draw##DrawCA", &ViewportManager::getInstance()->drawClickableArea); ImGui::SameLine(); 
	ImGui::Checkbox("Active##ActiveCA", &selectedObject->clickableAreaActive); 

	IMGUI_SPACE_SEPARATOR;

	ImGui::PushFont(App->moduleImGui->rudaBlackBig);
	ImGui::Text("Color");
	ImGui::PopFont();
 
	float color[4] =
	{
		selectedObject->GetClickableAreaColor().x,
		selectedObject->GetClickableAreaColor().y,
		selectedObject->GetClickableAreaColor().z,
		selectedObject->GetClickableAreaColor().w,
	};

	if (ImGui::ColorEdit4("", color))
		selectedObject->SetClickableAreaColor(float4(color[0], color[1], color[2], color[3]));
	
	ImGui::Spacing(); 
	if (selectedObject->HasVisuals())
	{
		ImGui::PushFont(App->moduleImGui->rudaBlackBig);
		ImGui::Text("Position");
		ImGui::PopFont();

		float2 posPerc = selectedObject->GetClickableAreaPosOne(); 
		if (ImGui::DragFloat("Horizontal", &selectedObject->GetClickableAreaPosOne().x, 0.01f, 0.05f, (1.0f - selectedObject->GetClickableAreaSizeOne().x)))
			selectedObject->FitObjectUtils();

		if (ImGui::DragFloat("Vertical", &selectedObject->GetClickableAreaPosOne().y, 0.01f, 0.05f, (1.0f - selectedObject->GetClickableAreaSizeOne().y)))
			selectedObject->FitObjectUtils();
	}

	ImGui::PushFont(App->moduleImGui->rudaBlackBig);
	ImGui::Text("Size");
	ImGui::PopFont();

	float widthLimit = 500.0f;
	float heigthLimit = 500.0f; 
	float dragInc = 1.0f;

	if (selectedObject->HasVisuals())
	{
		widthLimit = (1.0f - selectedObject->GetClickableAreaPosOne().x);
		heigthLimit = (1.0f - selectedObject->GetClickableAreaPosOne().y);
		dragInc = 0.01f; 
	}

	if(ImGui::DragFloat("Width", &selectedObject->GetClickableAreaSizeOne().x, dragInc, 0.05f, widthLimit))
		selectedObject->FitObjectUtils();

	if(ImGui::DragFloat("Heigth", &selectedObject->GetClickableAreaSizeOne().y, dragInc, 0.05f, heigthLimit))
		selectedObject->FitObjectUtils(); 
	
	IMGUI_SPACE_SEPARATOR;

	if (ImGui::Button("Edit From Viewport"))
	{
		selectedObject->GetClickableArea()->drawCornerPoints = true; 
	}
}

void ObjectPropertiesDockPanel::DrawActionSettings()
{
	if (selectedObject->selectedAction)
	{
		switch (selectedObject->selectedAction->GetActionType())
		{
		case AT_IMAGE:
			DrawToolImageSettings(); 
			break; 
		}
	}
}

void ObjectPropertiesDockPanel::DrawActionsList()
{
	ImGui::PushFont(App->moduleImGui->rudaBoldBig);
	ImGui::Text("Object Actions: ");
	ImGui::PopFont();

	ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0.12f, 0.14f, 0.17f, 1.00f));
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(5, 5));

	ImGui::BeginChild("##ToolsListObjectProperties", ImVec2(ImGui::GetContentRegionAvailWidth(), 200));

	int count = 0;
	for (auto& currentTool : selectedObject->GetActionsList()) 
	{
		ActionSelectableInfo selectableInfo = currentTool->GetActionSelectableInfo(); 
		DrawActionSelectable(selectableInfo, currentTool, count, 42);
		count++; 
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
		showToolDictionary = true; 
	}

	ImGui::SameLine();
	Texture* minusIconTex = (Texture*)ResourceManager::getInstance()->GetResource("MinusIconWhite");
	if (ImGui::ImageButton((ImTextureID)minusIconTex->GetTextureID(), ImVec2(18, 18)))
	{	
		Action* selectedTool = selectedObject->selectedAction; 

		if (selectedTool != nullptr)
		{
			selectedTool->CleanUp();
			selectedObject->DeleteAction(selectedTool->GetActionName());
			selectedTool = nullptr;
		}
	}

	ImGui::PopStyleVar();
	ImGui::PopStyleColor();	

	if (showToolDictionary)
	{
		ImGui::BeginChild("AddToolObjectProperties", ImVec2(ImGui::GetContentRegionAvailWidth(), 150)); 

		// Search Bar ---------------
		ImGui::InputTextWithHint("##SearchTool", "Search...", searchNewToolBuffer, IM_ARRAYSIZE(searchNewToolBuffer));
		ImGui::SameLine();

		Texture* filterIcon = (Texture*)ResourceManager::getInstance()->GetResource("FilterIcon");
		ImGui::Image((ImTextureID)filterIcon->GetTextureID(), ImVec2(22, 22));

		ImGui::Spacing();
		ImGui::Separator();
		ImGui::Spacing();

		// Tools Dictonary ----------
		ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0.12f, 0.14f, 0.17f, 1.00f));
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(2.0f, 2.0f));
			ImGui::BeginChild("##5ShowImage", ImVec2(ImGui::GetContentRegionAvailWidth(), 150));

			ActionSelectableInfo* newActionSelected = App->moduleManager->DrawActionDictionaryUI();
			if (newActionSelected != nullptr)
			{
				switch (newActionSelected->actionType)
				{
				case AT_IMAGE:
					selectedObject->AddDisplayImageAction("None");
					break;

				case AT_null:
					break;
				}

				showToolDictionary = false; 
			}

			ImGui::EndChild();
		ImGui::PopStyleColor();
		ImGui::PopStyleVar();

		ImGui::EndChild();
	}
}

void ObjectPropertiesDockPanel::DrawActionSelectable(ActionSelectableInfo& selectableInfo, Action*& currentTool, int posInList, int selectableHeigth = 42)
{
	ImGui::PushFont(App->moduleImGui->rudaBoldMid);

	Texture* imageIcon = (Texture*)ResourceManager::getInstance()->GetResource("ImageIcon");
	ImGui::SetCursorPos(ImVec2(10, 5 + (selectableHeigth * posInList)));
	ImGui::Image((ImTextureID)imageIcon->GetTextureID(), ImVec2(30, 30), ImVec2(0, 1), ImVec2(1, 0));

	ImGui::SetCursorPos(ImVec2(50, +(selectableHeigth * posInList)));
	if (ImGui::Selectable(selectableInfo.actionName.c_str(), currentTool->IsSelected(), ImGuiSelectableFlags_None, ImVec2(ImGui::GetContentRegionAvailWidth(), selectableHeigth))) {
		selectedObject->selectedAction = currentTool;
	}
	ImGui::PopFont();

	// Description -----
	ImGui::SetCursorPosY((selectableHeigth * posInList) + 20);
	ImGui::SetCursorPosX(ImGui::GetCursorPos().x + 52);

	ImGui::PushFont(App->moduleImGui->rudaRegularSmall);
	ImGui::TextWrapped(selectableInfo.actionDescription.c_str());
	ImGui::PopFont();
}

void ObjectPropertiesDockPanel::DrawObjectPlacementCH()
{
	ImGui::PushFont(App->moduleImGui->rudaBoldMid);
	if (ImGui::CollapsingHeader("Object Placement"))
	{
		ImGui::Spacing(); 
		float2 showPosition = App->moduleImGui->gameViewportDockPanel->GetMouseGamePos(); 

		float showPositionArr[2] = { selectedObject->transform->GetPosition().x, selectedObject->transform->GetPosition().y };
		float showRotationArr[2] = { selectedObject->transform->GetRotation().x, selectedObject->transform->GetRotation().y };
		float showScaleArr[2] = { selectedObject->transform->GetScale().x, selectedObject->transform->GetScale().y };

		selectedObject->transform->scalePrevTick = selectedObject->transform->GetScale(); 

		PUSH_FONT(App->moduleImGui->rudaRegularMid);
		if (ImGui::DragFloat2("Position", showPositionArr, 0.5f))
		{
			selectedObject->transform->SetPosition(showPositionArr[0], showPositionArr[1]);
			selectedObject->FitObjectUtils();
		}

		if (ImGui::DragFloat2("Rotation", showRotationArr))
		{
			selectedObject->transform->SetRotationEuler(float2(showRotationArr[0], showRotationArr[1]));
		}

		if (ImGui::DragFloat2("Scale", showScaleArr, 0.1f))
		{
			selectedObject->transform->SetScale(float2(showScaleArr[0], showScaleArr[1]));
			selectedObject->FitObjectUtils();
		}
		POP_FONT;
	}
	POP_FONT;
}


void ObjectPropertiesDockPanel::DrawToolImageSettings()
{
	DisplayImageAction* imageTool = (DisplayImageAction*)selectedObject->GetAction("Image");

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
			ImGui::TextColored(ImVec4(0.1f, 0.7f, 1.0f, 1.0f), "%d", imageToolTexture->GetHeigth());

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

