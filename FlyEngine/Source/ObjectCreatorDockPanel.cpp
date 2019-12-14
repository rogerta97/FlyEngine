#include "ObjectCreatorDockPanel.h"

#include "Application.h"
#include "TinyFileDialog.h"
#include "ModuleImGui.h"
#include "Action.h"
#include "ModuleRoomManager.h"
#include "MyFileSystem.h"
#include "ChangeRoomAction.h"
#include "Room.h"
#include "Texture.h"
#include "ImageImporter.h"
#include "DisplayImageAction.h"
#include "ViewportManager.h"
#include "ResourceManager.h"
#include "GameViewportDockPanel.h"

#include "FlyObject.h"
#include "imgui.h"

#include "mmgr.h"

ObjectCreatorDockPanel::ObjectCreatorDockPanel(bool isVisible) : DockPanel("Object Creator", isVisible)
{
	flyEngineSection = FLY_SECTION_null;
	dockPanelType = DOCK_OBJECT_CREATOR;

	previewClickableAreaTexture = nullptr;
	displayImageAction_Inv = nullptr; 

	clickableAreaPosPerc = float2(0, 0);
	clickableAreaSizePerc = float2(1.0f, 1.0f);
	previewTextureMaxSize = 220;
}

ObjectCreatorDockPanel::~ObjectCreatorDockPanel()
{

}

bool ObjectCreatorDockPanel::CleanUp()
{

	return true; 
}

bool ObjectCreatorDockPanel::Draw()
{
#pragma region secutiryChecks
	if (!DockPanel::Draw())
		return false;
#pragma endregion

	if (ImGui::Begin(panelName.c_str(), &visible, ImGuiWindowFlags_NoTitleBar))
	{
		ImGui::PushFont(App->moduleImGui->rudaBlackHuge);
		ImGui::Text("Object Creator:");
		ImGui::PopFont();

		ImGui::SameLine();
		DrawCreateButton();

		ImGui::Separator();
		ImGui::Spacing();

		ImGui::PushFont(App->moduleImGui->rudaRegularMid);
		ImGui::InputTextWithHint("Name##ObjectNaming", "Name...", newObjectName, 256 * sizeof(char));
		ImGui::PopFont();

		ImGui::InputTextMultiline("Description##ObjectDescription", newObjectDescription, 256 * sizeof(char), ImVec2(ImGui::GetContentRegionMax().x - 100, 100));

		static int selectedObjectType = 0;
		if (ImGui::Combo("Object Type", &selectedObjectType, "Action Object\0Inventory Item\0"))
		{

			switch (selectedObjectType)
			{
			case ACTION_OBJECT: 
				ResetObjectData();
				break; 

			case INVENTORY_ITEM: 
				ResetObjectData();
				clickableAreaActive = true; 
				break; 
			}
		}

		if (ImGui::Checkbox("Interactable", &creatingObject->IsInteractable()))
		{
			focusClickableAreaTab = true;
		}

		ImGui::Separator(); 

		if (selectedObjectType == ACTION_OBJECT)
		{
			ImGui::PushFont(App->moduleImGui->rudaBoldHuge);
			ImGui::Text("Object Attributes:");
			ImGui::PopFont();
			if (ImGui::BeginTabBar("MyTabBar", ImGuiTabBarFlags_None))
			{

				if (ImGui::BeginTabItem("Actions"))
				{
					DrawObjectCreator();
					ImGui::EndTabItem();
				}

				if (creatingObject->IsInteractable())
				{
					ImGuiTabItemFlags tabFlags = 0;
					if (focusClickableAreaTab)
					{
						tabFlags |= ImGuiTabItemFlags_SetSelected;
						focusClickableAreaTab = false;
					}

					if (ImGui::BeginTabItem("Clickable Area", 0, tabFlags))
					{
						DrawClickableAreaCreator();
						ImGui::EndTabItem();
					}
				}

				ImGui::EndTabBar();
			}
		}
		else if (selectedObjectType == INVENTORY_ITEM)
		{	
			ImGui::PushFont(App->moduleImGui->rudaBoldHuge);
			ImGui::Text("Item Inventory Attributes:");
			ImGui::PopFont();
			DrawInventorySettings();
		}
	}

	ImGui::End();

}

void ObjectCreatorDockPanel::DrawInventorySettings()
{
	// Preview of the object -----------------
	SPACING;
	PrintClickableAreaObjectVisuals(drawClickableAreaOver);
	SPACING; 

	// Input Text to Browse Image ------------
	static char inventoryBrowcseImageBuffer[512];
	ImGui::InputTextWithHint("", "Search...", inventoryBrowcseImageBuffer, IM_ARRAYSIZE(inventoryBrowcseImageBuffer));
	ImGui::SameLine();

	if (ImGui::Button("Creator##Browse Image"))
	{
		clickableAreaActive = true; 
		displayImageAction_Inv = creatingObject->AddDisplayImageAction("None"); 

		char const* lFilterPatterns[2] = { "*.jpg" , "*.png" };
		const char* path = tinyfd_openFileDialog("Load Image...", NULL, 2, lFilterPatterns, NULL, 0);

		if (path != NULL)
		{
			if (!ResourceManager::getInstance()->ExistResourcePath(path))
			{
				displayImageAction_Inv->SetTexture(ImageImporter::getInstance()->LoadTexture(path, false));
				ResourceManager::getInstance()->AddResource(displayImageAction_Inv->GetTexture(), displayImageAction_Inv->GetTexture()->GetName());
			}
			else
			{
				displayImageAction_Inv->SetTexture((Texture*)ResourceManager::getInstance()->GetResourceByPath(path));
			}

			strcpy(inventoryBrowcseImageBuffer, path);
		}
	}

	// Clickable Zone Settings ---------------
	ImGui::PushFont(App->moduleImGui->rudaBlackBig);
	ImGui::Text("Clickable Zone:");
	ImGui::PopFont();

	ImGui::Separator(); 
	ImGui::Spacing();

	if (displayImageAction_Inv != nullptr && displayImageAction_Inv->GetTexture() != nullptr)
	{
		if (ImGui::Checkbox("Draw", &drawClickableAreaOver))
		{

		}

		PUSH_CHILD_BG_COLOR;
		ImGui::BeginChild("InventoryCASett", ImVec2(ImGui::GetContentRegionAvail().x, 90));
		ImGui::Columns(2);

		ImGui::Text("Position");
		ImGui::DragFloat("X", &creatingObject->GetClickableAreaPosOne().x);
		ImGui::DragFloat("Y", &creatingObject->GetClickableAreaPosOne().y);

		ImGui::NextColumn();

		ImGui::Text("Dimensions");
		ImGui::DragFloat("Width", &creatingObject->GetClickableAreaSizeOne().x);
		ImGui::DragFloat("Heigth", &creatingObject->GetClickableAreaSizeOne().y);
		ImGui::EndChild(); 

		ImGui::PopStyleColor();
	}
	else
	{
		ImGui::PushFont(App->moduleImGui->rudaRegularSmall);
		ImGui::TextColored(ImVec4(0,1,1,1), "Inventory Items need images assigned to enable the clickable zone. Assign an image.");
		ImGui::PopFont();
	}

	// On Picked Actions ------------------
	ImGui::PushFont(App->moduleImGui->rudaBlackBig);
	ImGui::Text("Perform Action When Picked:");
	ImGui::PopFont();

	ImGui::Separator();
	ImGui::Spacing();

	static int onPickAction = 0; 
	ImGui::Combo("On Pick Action", &onPickAction, "None\0Do Sound\0Change Variable\0"); 

	PUSH_CHILD_BG_COLOR; 
	ImGui::BeginChild("OnPickActions", ImVec2(ImGui::GetContentRegionMax().x, 150)); 

	// Add On Pick Action

	ImGui::EndChild(); 
	ImGui::PopStyleColor(); 
}


void ObjectCreatorDockPanel::DrawPropertiesTab()
{
	ImGui::PushFont(App->moduleImGui->rudaBlackHuge);
	ImGui::Text("Object Creator:");
	ImGui::PopFont();

	ImGui::Separator();
}

void ObjectCreatorDockPanel::ResetObjectData()
{
	FLY_LOG("Object Data Reset"); 
	strcpy(newObjectName, "");
	strcpy(searchNewActionBuffer, "");
	selectedAction = nullptr;
	creatingObject = new FlyObject("Prev");
}

void ObjectCreatorDockPanel::DrawObjectCreator()
{
	DrawObjectActionsList();
	DrawAddAndDeleteActionButtons();
	DrawSelectedActionSettings();
}

void ObjectCreatorDockPanel::DrawObjectActionsList()
{
	ImGui::PushFont(App->moduleImGui->rudaBlackBig);
	ImGui::Text("Add Actions: ");
	ImGui::PopFont();

	ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0.12f, 0.14f, 0.17f, 1.00f));
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(5, 5));

	ImGui::BeginChild("##AttributesChild", ImVec2(ImGui::GetContentRegionAvailWidth(), 200));

	int pos = 0;
	for (auto& currentAction : creatingObject->GetActionsList())
	{
		ActionSelectableInfo selectableInfo = currentAction->GetActionSelectableInfo();
		DrawSelectable(selectableInfo, currentAction->IsSelected(), pos, 42, currentAction);
		pos++;
	}

	ImGui::EndChild();

	ImGui::PopStyleVar();
	ImGui::PopStyleColor();
}

void ObjectCreatorDockPanel::DrawSelectable(ActionSelectableInfo selectableInfo, bool& isSelected, int posInList, int selectableHeight = 42, Action * currentAction = nullptr)
{
	ImGui::PushFont(App->moduleImGui->rudaBoldMid);

	Texture* imageIcon = App->moduleManager->GetIconFromActionType(selectableInfo.actionType);
	ImGui::SetCursorPos(ImVec2(10, 5 + (selectableHeight * posInList)));
	ImGui::Image((ImTextureID)imageIcon->GetTextureID(), ImVec2(30, 30), ImVec2(0, 1), ImVec2(1, 0));

	ImGui::SetCursorPos(ImVec2(50, (selectableHeight * posInList) + 4));
	if (ImGui::Selectable(selectableInfo.actionName.c_str(), &isSelected, ImGuiSelectableFlags_None, ImVec2(ImGui::GetContentRegionMax().x, selectableHeight - 3))) {
		creatingObject->SetSelectedAction(selectableInfo.actionType);
		selectedAction = currentAction;
	}
	ImGui::PopFont();

	// Description -----
	ImGui::SetCursorPosY((selectableHeight * posInList) + 24);
	ImGui::SetCursorPosX(ImGui::GetCursorPos().x + 52);

	ImGui::PushFont(App->moduleImGui->rudaRegularSmall);
	ImGui::TextWrapped(selectableInfo.actionDescription.c_str());
	ImGui::PopFont();
}

void ObjectCreatorDockPanel::DrawSelectedActionSettings()
{
	if (selectedAction)
	{
		ImGui::Spacing();

		switch (selectedAction->GetActionType())
		{
		case AT_DISPLAY_IMAGE:
			DrawDisplayImageSettings();
			break;

		case AT_CHANGE_ROOM:
			DrawChangeRoomActionSettings();
			break;
		}
	}
}

void ObjectCreatorDockPanel::DrawChangeRoomActionSettings()
{
	ImGui::PushFont(App->moduleImGui->rudaBoldHuge);
	ImGui::Text("Change Room Attributes:");
	ImGui::PopFont();

	ImGui::Separator();

	ChangeRoomAction* changeRoomAction = (ChangeRoomAction*)this->selectedAction;

	ImGui::PushFont(App->moduleImGui->rudaBoldBig);
	ImGui::Text("Action Happens On:");
	ImGui::PopFont();

	ImGui::PushFont(App->moduleImGui->rudaRegularMid);
	ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0.12f, 0.14f, 0.17f, 1.00f));
	ImGui::BeginChild("##OccChild", ImVec2(ImGui::GetContentRegionAvailWidth(), 100));

	ImGui::SetCursorPos(ImVec2(5, 8));
	ImGui::Checkbox("Scene Enter", &changeRoomAction->IsOccSceneEnter());
	ImGui::SetCursorPos(ImVec2(5, 38));
	ImGui::Checkbox("Scene Leave", &changeRoomAction->IsOccSceneLeave());
	ImGui::SetCursorPos(ImVec2(5, 68));
	ImGui::Checkbox("Object Clicked", &changeRoomAction->IsOccObjectClicked());

	ImGui::SameLine();
	if (ImGui::SmallButton("Adjust Clickable Area"))
	{
		focusClickableAreaTab = true;
	}

	ImGui::Spacing();
	ImGui::EndChild();

	ImGui::PopFont();
	ImGui::PopStyleColor();

	IMGUI_SPACED_SEPARATOR;

	ImGui::PushFont(App->moduleImGui->rudaBoldBig);
	ImGui::Text("Change Room Settings: ");
	ImGui::PopFont();

	string* rooms = App->moduleRoomManager->GetRoomsAsCombo();
	const char* roomsToCombo[] = { "None", rooms[0].c_str(), rooms[1].c_str(), rooms[2].c_str() };
	static int ci = 0;
	ImGui::ComboArray("Destination", &ci, roomsToCombo, IM_ARRAYSIZE(roomsToCombo));

}

void ObjectCreatorDockPanel::OnAddActionButtonClicked()
{
	if (showActionDictionary)
	{
		ImGui::Separator();
		ImGui::Spacing();

		// Search Bar ---------------
		ImGui::InputText("##SearchTool", searchNewActionBuffer, IM_ARRAYSIZE(searchNewActionBuffer));
		ImGui::SameLine();

		Texture* filterIcon = (Texture*)ResourceManager::getInstance()->GetResource("FilterIcon");
		ImGui::Image((ImTextureID)filterIcon->GetTextureID(), ImVec2(22, 22));

		ImGui::Spacing();

		ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0.12f, 0.14f, 0.17f, 1.00f));
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(2.0f, 2.0f));
		ImGui::BeginChild("##4ShowImage", ImVec2(ImGui::GetContentRegionAvailWidth(), 150));

		// Tools Dictonary ----------
		ActionSelectableInfo* newActionSelected = App->moduleManager->DrawActionDictionaryUI();
		if (newActionSelected != nullptr)
		{
			switch (newActionSelected->actionType)
			{
			case AT_DISPLAY_IMAGE:
				selectedAction = creatingObject->AddDisplayImageAction(std::string(MyFileSystem::getInstance()->GetIconsDirectory() + "EmptyObject.png").c_str());

				break;

			case AT_CHANGE_ROOM:
				creatingObject->AddChangeRoomAction();
				break;

			case AT_null:
				break;
			}

			showActionDictionary = false;
			newActionSelected = nullptr; 
		}
		ImGui::EndChild();
		ImGui::PopStyleColor();
		ImGui::PopStyleVar();
		ImGui::Separator();
	}
}

void ObjectCreatorDockPanel::Close()
{
	selectedAction = nullptr;
	visible = false;
}

void ObjectCreatorDockPanel::DrawClickableAreaCreator()
{
	ImGui::PushFont(App->moduleImGui->rudaBlackBig);
	ImGui::Text("Preview:");
	ImGui::PopFont();

	ImGui::SameLine();
	ImGui::SetCursorPosX(ImGui::GetContentRegionAvailWidth() + 10);
	if (ImGui::Checkbox("Active", &clickableAreaActive))
	{
		if (clickableAreaActive && previewClickableAreaTexture != nullptr)
		{
			float2 textureSize = float2(previewClickableAreaTexture->GetWidth(), previewClickableAreaTexture->GetHeigth());
		}
	}

	PrintClickableAreaObjectVisuals();
	DrawClickableAreaSettings();
}

void ObjectCreatorDockPanel::DrawClickableAreaSettings()
{
	if (clickableAreaActive)
	{
		float2 posLimit = float2(1, 1);
		float2 sizeLimit = float2(1, 1);

		if (!creatingObject->HasVisuals())
		{
			ImGui::Separator();

			ImGui::PushFont(App->moduleImGui->rudaRegularSmall);
			ImGui::PushTextWrapPos(ImGui::GetContentRegionAvailWidth() + 5);
			ImGui::TextColored(ImVec4(1, 1, 0.2f, 0.8f), "The object being created has no actions with visible content. Clickable Area position will fit the center of the object");
			ImGui::PopTextWrapPos();
			ImGui::PopFont();

			ImGui::Separator();

			ImGui::DragFloat("Width", &clickableAreaSizePerc.x, 1.0f, 0.1f, App->moduleImGui->gameViewportDockPanel->GetViewportSize().x);
			ImGui::DragFloat("Height", &clickableAreaSizePerc.y, 1.0f, 0.1f, App->moduleImGui->gameViewportDockPanel->GetViewportSize().y);
		}
		else
		{
			ImGui::PushFont(App->moduleImGui->rudaBlackBig);
			ImGui::Text("Position:");
			ImGui::PopFont();

			posLimit = float2(1, 1) - clickableAreaSizePerc;
			if (clickableAreaSizePerc.x > sizeLimit.x)
			{
				sizeLimit.x = 1 - clickableAreaPosPerc.x;
			}

			if (clickableAreaSizePerc.y > sizeLimit.y)
			{
				sizeLimit.y = 1 - clickableAreaPosPerc.x;
			}

			ImGui::DragFloat("Horizontal", &clickableAreaPosPerc.x, 0.005f, 0.05f, posLimit.x);
			ImGui::DragFloat("Vertical", &clickableAreaPosPerc.y, 0.005f, 0.05f, posLimit.y);

			ImGui::PushFont(App->moduleImGui->rudaBlackBig);
			ImGui::Text("Size:");
			ImGui::PopFont();

			float2 viewportSize = App->moduleImGui->gameViewportDockPanel->GetViewportSize();
			sizeLimit = float2(1, 1) - clickableAreaPosPerc;
			if (clickableAreaPosPerc.x > posLimit.x)
			{
				posLimit.x = 1 - clickableAreaSizePerc.x;
			}

			if (clickableAreaPosPerc.y > posLimit.y)
			{
				posLimit.y = 1 - clickableAreaSizePerc.x;
			}

			ImGui::DragFloat("Width", &clickableAreaSizePerc.x, 0.005f, 0.05f, sizeLimit.x);
			ImGui::DragFloat("Height", &clickableAreaSizePerc.y, 0.005f, 0.05f, sizeLimit.y);

			IMGUI_SPACED_SEPARATOR;
		}
	}
}

void ObjectCreatorDockPanel::PrintClickableAreaObjectVisuals(bool drawClickableArea)
{

	ImGui::PushFont(App->moduleImGui->rudaBoldMid);
	ImGui::Text("Preview:");
	ImGui::PopFont();

	ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0.12f, 0.14f, 0.17f, 1.00f));
	ImGui::BeginChild("ShowClickableArea", ImVec2(ImGui::GetContentRegionAvailWidth(), previewTextureMaxSize));
	if (!creatingObject->HasVisuals())
	{
		// Background
		ImGui::SetCursorPos(ImVec2(ImGui::GetContentRegionAvailWidth() / 2 - (previewTextureMaxSize / 2), 0));
		ImGui::Image(0, ImVec2(previewTextureMaxSize, previewTextureMaxSize));

		// Show No Visual Text
		ImGui::PushFont(App->moduleImGui->rudaBoldBig);
		
		if (clickableAreaActive) {
			ImGui::SetCursorPos(ImVec2(ImGui::GetContentRegionAvailWidth() / 2 - 45, 98));
			ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.7f, 0.8f), "NO VISUALS");
		}
		else {
			ImGui::SetCursorPos(ImVec2(ImGui::GetContentRegionAvailWidth() / 2 - 40, 98));
			ImGui::TextColored(ImVec4(1.0f, 1.0f, 1.0f, 0.5f), "INACTIVE");
		}

		ImGui::PopFont();
	}
	else
	{
		DrawPrevTextureCA(drawClickableArea);
	}

	ImGui::EndChild();
	ImGui::PopStyleColor();
}

void ObjectCreatorDockPanel::DrawPrevTextureCA(bool drawClickableArea)
{
	// Prev Texture
	DisplayImageAction* displayImageAction = (DisplayImageAction*)creatingObject->GetAction(AT_DISPLAY_IMAGE);
	if (displayImageAction != nullptr)
	{
		previewClickableAreaTexture = displayImageAction->GetTexture();

		if (previewClickableAreaTexture == nullptr)
			return;

		ImVec2 prevTextureSize;
		GetTextureSizeFitted(prevTextureSize);

		float yOffset = ImGui::GetContentRegionAvail().y / 2 - (prevTextureSize.y / 2);
		float2 imageTopLeft = float2(ImGui::GetContentRegionAvailWidth() / 2 - (previewTextureMaxSize / 2), yOffset);

		ImGui::SetCursorPos(ImVec2(imageTopLeft.x, imageTopLeft.y));
		ImGui::Image((ImTextureID)previewClickableAreaTexture->GetTextureID(), prevTextureSize);

		if (clickableAreaActive && drawClickableArea) {
			DrawPreviewClickableAreaOnTexture(imageTopLeft, float2(prevTextureSize.x, prevTextureSize.y));
		}
	}
}

void ObjectCreatorDockPanel::GetTextureSizeFitted(ImVec2& prevTextureSize)
{
	prevTextureSize.x = previewClickableAreaTexture->GetWidth();
	prevTextureSize.y = previewClickableAreaTexture->GetHeigth();

	if (prevTextureSize.x <= prevTextureSize.y)
	{
		prevTextureSize.y = previewTextureMaxSize;
		prevTextureSize.x = previewTextureMaxSize / previewClickableAreaTexture->GetAspectRatio();
	}
	else
	{
		prevTextureSize.x = previewTextureMaxSize;
		prevTextureSize.y = previewTextureMaxSize / previewClickableAreaTexture->GetAspectRatio();
	}
}

void ObjectCreatorDockPanel::DrawPreviewClickableAreaOnTexture(float2 textureTopLeft, float2 prevTextureSize)
{
	float2 clickableAreaPos = float2(prevTextureSize.x * clickableAreaPosPerc.x, prevTextureSize.y * clickableAreaPosPerc.y);
	float2 clickableAreaSize = float2(prevTextureSize.x * clickableAreaSizePerc.x, prevTextureSize.y * clickableAreaSizePerc.y);

	float2 clickableAreaTopLeft = textureTopLeft + clickableAreaPos;
	ImGui::SetCursorPos(ImVec2(clickableAreaTopLeft.x, clickableAreaTopLeft.y));

	Texture* colorTexture = (Texture*)ResourceManager::getInstance()->GetResource("ClickableAreaPreviewColor");
	ImGui::Image((ImTextureID)colorTexture->GetTextureID(), ImVec2(clickableAreaSize.x, clickableAreaSize.y));
}

void ObjectCreatorDockPanel::DrawCreateButton()
{
	ImGui::SetCursorPosX(ImGui::GetContentRegionMax().x - 100);
	ImGui::PushFont(App->moduleImGui->rudaBlackMid);
	if (ImGui::Button("Create", ImVec2(100, 30)))
	{
		std::string newObjectNameStr(newObjectName);
		if (newObjectNameStr.empty()) {
			FLY_ERROR("Object with no name can not be created");
			ImGui::PopFont();
			return;
		}
		else
		{
			AddCreatingObject();
		}

		Close();
	}

	ImGui::PopFont();
}

void ObjectCreatorDockPanel::AddCreatingObject()
{
	// Clickable Area
	if (clickableAreaActive)
	{
		if (!creatingObject->HasVisuals())
		{
			creatingObject->CreateClickableArea(clickableAreaPosPerc, clickableAreaSizePerc, true);
			ViewportManager::getInstance()->drawClickableArea = true;
		}
		else
		{
			creatingObject->CreateClickableArea(clickableAreaPosPerc, clickableAreaSizePerc);
			ViewportManager::getInstance()->drawClickableArea = false;
		}
		creatingObject->clickableAreaActive = true;
	}

	creatingObject->SetClickableAreaPosOne(clickableAreaPosPerc);
	creatingObject->SetClickableAreaSizeOne(clickableAreaSizePerc);

	// Add Object
	creatingObject->SetName(newObjectName);
	App->moduleRoomManager->GetSelectedRoom()->AddFlyObject(creatingObject);

	App->moduleRoomManager->GetSelectedRoom()->SetSelectedObject(creatingObject);
	App->moduleImGui->gameViewportDockPanel->SetGizmoMode(GizmoMode::GIZMO_SELECT);
	creatingObject->FitObjectUtils();
}

void ObjectCreatorDockPanel::DrawDisplayImageSettings()
{
	ImGui::PushFont(App->moduleImGui->rudaBoldHuge);
	ImGui::Text("Display Image Attributes:");
	ImGui::PopFont();

	ImGui::Separator();

	static char buf[256] = "";
	DisplayImageAction* selectedImageAction = (DisplayImageAction*)this->selectedAction;

	// Object Occurrence -----------------------
	selectedImageAction->DrawActionOccurenceCheckboxes();

	IMGUI_SPACE_SEPARATOR;

	// Settings ---------------------------------
	//if (selectedImageAction->GetTexture() == nullptr)
	//	selectedImageAction->SetTexture((Texture*)ResourceManager::getInstance()->GetResource("EmptyObject"));
	//else
	//	selectedImageAction->SetTexture((Texture*)ResourceManager::getInstance()->GetResource(selectedImageAction->GetTexture()->GetName()));

	float aspect_ratio = selectedImageAction->GetTexture()->GetAspectRatio();
	float previewQuadWidth = 150;
	float previewQuadHeight = previewQuadWidth / aspect_ratio;

	PUSH_FONT(App->moduleImGui->rudaRegularMid);
	ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0.12f, 0.14f, 0.17f, 1.00f));

	int childHeight = previewQuadHeight + 20;

	PUSH_FONT(App->moduleImGui->rudaBoldBig);
	ImGui::Text("Image Settings:");
	POP_FONT;

	ImGui::BeginChild("##4ShowImage", ImVec2(ImGui::GetContentRegionAvailWidth(), childHeight));

	ImGui::Columns(2);
	ImGui::SetColumnWidth(0, previewQuadWidth + 10);

	ImGui::Spacing();
	ImGui::Image((ImTextureID)selectedImageAction->GetTexture()->GetTextureID(), ImVec2(previewQuadWidth, previewQuadHeight));

	ImGui::NextColumn();

	ImGui::Spacing();
	ImGui::Text("Name: "); ImGui::SameLine();
	ImGui::TextColored(ImVec4(0.1f, 0.7f, 1.0f, 1.0f), "%s", selectedImageAction->GetTexture()->GetName().c_str());

	ImGui::Text("Width: "); ImGui::SameLine();
	ImGui::TextColored(ImVec4(0.1f, 0.7f, 1.0f, 1.0f), "%d", selectedImageAction->GetTexture()->GetWidth());

	ImGui::Text("Height: "); ImGui::SameLine();
	ImGui::TextColored(ImVec4(0.1f, 0.7f, 1.0f, 1.0f), "%d", selectedImageAction->GetTexture()->GetHeigth());

	Texture* searchTexture = (Texture*)ResourceManager::getInstance()->GetResource("SearchIcon");
	if (ImGui::Button("Change Image"))
	{
		char const* lFilterPatterns[2] = { "*.jpg" , "*.png" };
		const char* path = tinyfd_openFileDialog("Load Image...", NULL, 2, lFilterPatterns, NULL, 0);

		if (path != NULL)
		{
			if (!ResourceManager::getInstance()->ExistResourcePath(path))
			{
				selectedImageAction->SetTexture(ImageImporter::getInstance()->LoadTexture(path, false));
				ResourceManager::getInstance()->AddResource(selectedImageAction->GetTexture(), selectedImageAction->GetTexture()->GetName());
			}
			else
			{
				selectedImageAction->SetTexture((Texture*)ResourceManager::getInstance()->GetResourceByPath(path));
			}

			strcpy(buf, path);
			FLY_LOG("Player Opened %s", path);
		}

	}
	ImGui::PopStyleColor();
	ImGui::EndChild();

	POP_FONT;

}

void ObjectCreatorDockPanel::DrawAddAndDeleteActionButtons()
{
	ImGui::Spacing();
	ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(2, 2));

	ImGui::SetCursorPosY(ImGui::GetCursorPosY() - 5);
	Texture* plusIconTex = (Texture*)ResourceManager::getInstance()->GetResource("PlusIconWhite");

	if (ImGui::ImageButton((ImTextureID)plusIconTex->GetTextureID(), ImVec2(30, 30)))
	{
		showActionDictionary = true;
	}

	ImGui::SameLine();
	Texture* minusIconTex = (Texture*)ResourceManager::getInstance()->GetResource("MinusIconWhite");
	if (ImGui::ImageButton((ImTextureID)minusIconTex->GetTextureID(), ImVec2(30, 30)))
	{
		if (selectedAction != nullptr) {
			creatingObject->DeleteAction(selectedAction->GetActionName());
		}
	}

	ImGui::PopStyleVar();
	ImGui::Spacing();

	// Callbacks for buttons 
	OnAddActionButtonClicked();
}
