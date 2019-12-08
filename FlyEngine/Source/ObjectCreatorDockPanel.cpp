#include "ObjectCreatorDockPanel.h"

#include "Application.h"
#include "TinyFileDialog.h"
#include "ModuleImGui.h"
#include "Action.h"
#include "ModuleRoomManager.h"
#include "MyFileSystem.h"
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

	clickableAreaPosPerc = float2(0, 0);
	clickableAreaSizePerc = float2(1.0f, 1.0f); 
	previewTextureMaxSize = 220; 
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

		ImGui::SameLine();

		if (ImGui::Checkbox("Interactable", &creatingObject->IsInteractable()))
		{

		}

		ImGui::Spacing();
		ImGui::InputTextMultiline("Description##ObjectDescription", newObjectDescription, 256 * sizeof(char), ImVec2(ImGui::GetContentRegionMax().x - 100, 100));

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
				if (ImGui::BeginTabItem("Clickable Area"))
				{
					DrawClickableAreaCreator(); 
					ImGui::EndTabItem();
				}				
			}

			ImGui::EndTabBar();
		}	
	}

	ImGui::End(); 
	
}


void ObjectCreatorDockPanel::DrawPropertiesTab()
{
	ImGui::PushFont(App->moduleImGui->rudaBlackHuge);
	ImGui::Text("Object Creator:");
	ImGui::PopFont();

	ImGui::Separator();

	static bool t;
	ImGui::Checkbox("Interactable", &t);
}

void ObjectCreatorDockPanel::ResetObjectData()
{
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

void ObjectCreatorDockPanel::DrawSelectable(ActionSelectableInfo selectableInfo, bool& isSelected, int posInList, int selectableHeight = 42, Action* currentAction = nullptr)
{
	ImGui::PushFont(App->moduleImGui->rudaBoldMid);

	Texture* imageIcon = App->moduleManager->GetIconFromActionType(selectableInfo.actionType);
	ImGui::SetCursorPos(ImVec2(10, 5 + (selectableHeight * posInList)));
	ImGui::Image((ImTextureID)imageIcon->GetTextureID(), ImVec2(30, 30), ImVec2(0, 1), ImVec2(1, 0));

	ImGui::SetCursorPos(ImVec2(50, (selectableHeight * posInList) + 4));
	if (ImGui::Selectable(selectableInfo.actionName.c_str(), &isSelected, ImGuiSelectableFlags_None, ImVec2(ImGui::GetContentRegionMax().x, selectableHeight - 3))) {
		creatingObject->selectedAction = currentAction;
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
		ImGui::Separator();
		ImGui::Separator();

	/*	ImGui::PushFont(App->moduleImGui->rudaBoldHuge);
		string settingsName = selectedAction->GetActionName() + string(" Settings:");
		ImGui::Text(settingsName.c_str());
		ImGui::PopFont(); */
	 
		switch (selectedAction->GetActionType())
		{
		case AT_IMAGE:
			DrawDisplayImageSettings();
			break;

		case AT_CHANGE_SCENE:

			
			string* rooms = App->moduleRoomManager->GetRoomsAsCombo(); 
			
			const char* roomsToCombo[] = { rooms[0].c_str(), rooms[1].c_str(), rooms[2].c_str() };
			static int ci = 0; 
			ImGui::Combo("testing", &ci, roomsToCombo);

			static char destinationRoomName[256];
			ImGui::InputText("Prosivisional", destinationRoomName, IM_ARRAYSIZE(destinationRoomName));
			break;
		}
	}
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
			case AT_IMAGE:
				selectedAction = creatingObject->AddDisplayImageAction(std::string(MyFileSystem::getInstance()->GetIconsDirectory() + "EmptyObject.png").c_str());	
				break;

			case AT_CHANGE_SCENE:
				creatingObject->AddChangeRoomAction();
				break;

			case AT_null:
				break;
			}

			showActionDictionary = false; 
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
	PrintClickableAreaObjectVisuals();
	DrawClickableAreaSettings();
}

void ObjectCreatorDockPanel::DrawClickableAreaSettings()
{
	if (clickableAreaActive)
	{
		float2 posLimit = float2(1,1);
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

void ObjectCreatorDockPanel::PrintClickableAreaObjectVisuals()
{
	ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0.12f, 0.14f, 0.17f, 1.00f));

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

	ImGui::BeginChild("ShowClickableArea", ImVec2(ImGui::GetContentRegionAvailWidth(), previewTextureMaxSize));

	if (!creatingObject->HasVisuals())
	{
		// Background
		ImGui::SetCursorPos(ImVec2(ImGui::GetContentRegionAvailWidth() / 2 - (previewTextureMaxSize / 2), 0));
		ImGui::Image(0, ImVec2(previewTextureMaxSize, previewTextureMaxSize));
		
		// Show No Visual Text
		ImGui::PushFont(App->moduleImGui->rudaBoldBig);
		ImGui::SetCursorPos(ImVec2(ImGui::GetContentRegionAvailWidth() / 2 - 50, 100));

		if(clickableAreaActive)
			ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.7f, 0.8f), "NO VISUALS");
		else
			ImGui::TextColored(ImVec4(1.0f, 1.0f, 1.0f, 0.5f), "INACTIVE");

		ImGui::PopFont(); 
	}
	else
	{
		DrawPrevTextureCA();
	}

	ImGui::EndChild();
	ImGui::PopStyleColor();
}

void ObjectCreatorDockPanel::DrawPrevTextureCA()
{
	// Prev Texture
	DisplayImageAction* displayImageAction = (DisplayImageAction*)creatingObject->GetAction(AT_IMAGE);
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
		
		if (clickableAreaActive) {
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
	ImGui::SetCursorPosX(ImGui::GetContentRegionMax().x - 70); 
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
}

void ObjectCreatorDockPanel::DrawDisplayImageSettings()
{	
	static char buf[256] = "";
	DisplayImageAction* selectedImageAction = (DisplayImageAction*)this->selectedAction;

	// Object Occurrence -----------------------
	selectedImageAction->DrawActionOccurenceCheckboxes();

	IMGUI_SPACE_SEPARATOR;

	if (selectedImageAction->GetTexture() == nullptr)
		selectedImageAction->SetTexture((Texture*)ResourceManager::getInstance()->GetResource("EmptyObject"));
	else
		selectedImageAction->SetTexture((Texture*)ResourceManager::getInstance()->GetResource(selectedImageAction->GetTexture()->GetName()));

	float aspect_ratio = selectedImageAction->GetTexture()->GetAspectRatio();
	float previewQuadWidth = 150;
	float previewQuadHeight = previewQuadWidth / aspect_ratio;

	PUSH_FONT(App->moduleImGui->rudaRegularMid);
	ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0.12f, 0.14f, 0.17f, 1.00f));

	int childHeight = previewQuadHeight + 20;

	// Settings ---------------------------------
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
