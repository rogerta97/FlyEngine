#include "ObjectPropertiesDockPanel.h"
#include "Application.h"
#include "ModuleImGui.h"
#include "ModuleManager.h"
#include "imgui.h"
#include "Action.h"
#include "ModuleRoomManager.h"
#include "ModifyVariableAction.h"
#include "Gizmos.h"
#include "ResourceManager.h"
#include "ChangeRoomAction.h"
#include "DisplayImageAction.h"
#include "RoomUIHandler.h"
#include "AudioClip.h"
#include "GameViewportDockPanel.h"
#include "UI_Image.h"
#include "UI_Element.h"
#include "ImageImporter.h"
#include "UI_Button.h"
#include "Action.h"
#include "EmitSoundAction.h"
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
	if (!DockPanel::Draw() || App->moduleRoomManager->GetSelectedRoom() == nullptr)
		return false;
#pragma endregion

	if (ImGui::Begin(panelName.c_str(), &visible)) 
	{
		switch (ViewportManager::getInstance()->editRoomMode)
		{
		case EditRoomMode::EDIT_ROOM_OBJECTS:
			DrawFlyObjectProperties();	
			break; 

		case EditRoomMode::EDIT_ROOM_UI:
			DrawUIElementProperties();		
			break; 
		}
	}

	ImGui::End();
	return true;
}

void ObjectPropertiesDockPanel::DrawUIElementProperties()
{
	RoomUIHandler* roomUIHandler = App->moduleRoomManager->GetSelectedRoom()->roomUIHandler;
	UI_Element* selectedUIElement = roomUIHandler->GetSelectedElement();

	if (selectedUIElement != nullptr)
	{
		DrawFixedPartObjectUI(selectedUIElement->GetHolderObject());

		switch (selectedUIElement->uiElementType)
		{
		case UI_IMAGE:
			DrawUIImageProperties(selectedUIElement);
			break; 

		case UI_BUTTON:
			DrawUIButtonProperties(selectedUIElement);
			break;
		}
	}
	else {
		ImGui::Text("Select something dude :)");
	}
}

void ObjectPropertiesDockPanel::DrawUIButtonProperties(UI_Element* selectedUIElement)
{
	UI_Button* selectedButton = (UI_Button*)selectedUIElement;
	static char uiImageNameBuffer[256] = "";

	if (ImGui::CollapsingHeader("UI Button Properties", ImGuiTreeNodeFlags_DefaultOpen))
	{
		ImGui::Spacing();

		ImGui::PushFont(App->moduleImGui->rudaBlackBig);
		ImGui::Text("Button Image:"); 
		ImGui::PopFont(); 

		DrawButtonMainImagePreview(selectedButton);

		// Button Reaction ---------------------------------
		ImGui::PushFont(App->moduleImGui->rudaBlackBig);
		ImGui::Text("Button Reaction:");
		ImGui::PopFont();

		int childHeight = 115;
		static int reactionTypeSelected = 0; 

		if (reactionTypeSelected == 1)
			childHeight = 270; 
		
		ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0.12f, 0.14f, 0.17f, 1.00f));
		ImGui::BeginChild("ButtonReaction", ImVec2(ImGui::GetContentRegionAvailWidth(), childHeight));

		INC_CURSOR_10
		if (ImGui::Combo("Behaviour", &reactionTypeSelected, "Color Tint\0Swap Image"))
		{

		}

		if (reactionTypeSelected == 0)
			DrawColorTintSection();
		
		if (reactionTypeSelected == 1)
		{
			INC_CURSOR_10;
			ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0.082f,  0.105f,  0.13f, 1.0f));
			ImGui::BeginChild("OnMouseOverTexture", ImVec2(ImGui::GetContentRegionAvailWidth()- 10, 100));

			int maxImageSize = 80; 
			ImVec2 imageDimensions = ImVec2(maxImageSize, maxImageSize);
			ImTextureID mouseOverID = 0; 
			ImGui::Columns(2); 
			ImGui::SetColumnWidth(0, 100); 

			if (selectedButton->GetMouseOverTexture() != nullptr)
			{
				imageDimensions = selectedButton->GetMouseOverTexture()->GetImageSizeInSquare(ImVec2(maxImageSize, maxImageSize));
				mouseOverID = (ImTextureID)selectedButton->GetMouseOverTexture()->GetTextureID();
			}
			
			ImGui::SetCursorPos(ImVec2(ImGui::GetContentRegionAvailWidth() / 2 - (imageDimensions.x / 2) + 8, ImGui::GetContentRegionAvail().y / 2 - (imageDimensions.y / 2)));
			ImGui::Image(mouseOverID, imageDimensions);

			ImGui::NextColumn();

			INC_CURSOR_10;
			ImGui::PushFont(App->moduleImGui->rudaBlackBig);
			ImGui::Text("On Mouse Over Image:");
			ImGui::PopFont();

			static char searchImageOver[256];

			if (selectedButton->GetMouseOverTexture() != nullptr)
				strcpy(searchImageOver, selectedButton->GetMouseOverTexture()->GetName().c_str());
			
			ImGui::Spacing();
			ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 5);
			ImGui::SetNextItemWidth(ImGui::GetContentRegionAvailWidth() - 70);
			ImGui::InputTextWithHint("", "Search Image...", searchImageOver, IM_ARRAYSIZE(searchImageOver));

			if (ImGui::BeginDragDropTarget())
			{
				if (const ImGuiPayload * payload = ImGui::AcceptDragDropPayload("drag_resource"))
				{
					int* selectedResourceUID = (int*)payload->Data;
					Resource* resourceDropped = ResourceManager::getInstance()->GetResource(*selectedResourceUID);

					if (resourceDropped->GetType() == RESOURCE_TEXTURE)
					{
						Texture* textureDropped = (Texture*)resourceDropped;
						selectedButton->SetMouseOverTexture(textureDropped);
					}
				}

				ImGui::EndDragDropTarget();
			}
			
			ImGui::SameLine();
			if (ImGui::Button("Search##SearchOverImage"))
			{

			}

			ImGui::EndChild();

			INC_CURSOR_10;
			ImGui::BeginChild("OnMouseClickedTexture", ImVec2(ImGui::GetContentRegionAvailWidth() - 10, 100));

			ImGui::Columns(2);
			ImGui::SetColumnWidth(0, 100);

			maxImageSize = 80;
			imageDimensions = ImVec2(maxImageSize, maxImageSize);
			ImTextureID mouseClickedID = 0;
			ImGui::Columns(2);
			ImGui::SetColumnWidth(0, 100);

			if (selectedButton->GetMouseClickedTexture() != nullptr)
			{
				imageDimensions = selectedButton->GetMouseClickedTexture()->GetImageSizeInSquare(ImVec2(maxImageSize, maxImageSize));
				mouseClickedID = (ImTextureID)selectedButton->GetMouseClickedTexture()->GetTextureID();
			}

			ImGui::SetCursorPos(ImVec2(ImGui::GetContentRegionAvailWidth() / 2 - (imageDimensions.x / 2) + 8, ImGui::GetContentRegionAvail().y / 2 - (imageDimensions.y / 2)));
			ImGui::Image(mouseClickedID, imageDimensions);

			ImGui::NextColumn();

			INC_CURSOR_10;
			ImGui::PushFont(App->moduleImGui->rudaBlackBig);
			ImGui::Text("On Mouse Clicked Image:");
			ImGui::PopFont();

			static char searchImageClickBuffer[256];

			if (selectedButton->GetMouseClickedTexture() != nullptr)
				strcpy(searchImageClickBuffer, selectedButton->GetMouseClickedTexture()->GetName().c_str());

			ImGui::Spacing();
			ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 5);
			ImGui::SetNextItemWidth(ImGui::GetContentRegionAvailWidth() - 70);
			ImGui::InputTextWithHint("", "Search Image...", searchImageClickBuffer, IM_ARRAYSIZE(searchImageClickBuffer));

			if (ImGui::BeginDragDropTarget())
			{
				if (const ImGuiPayload * payload = ImGui::AcceptDragDropPayload("drag_resource"))
				{
					int* selectedResourceUID = (int*)payload->Data;
					Resource* resourceDropped = ResourceManager::getInstance()->GetResource(*selectedResourceUID);

					if (resourceDropped->GetType() == RESOURCE_TEXTURE)
					{
						Texture* textureDropped = (Texture*)resourceDropped;
						selectedButton->SetMouseClickedTexture(textureDropped);
					}
				}

				ImGui::EndDragDropTarget();
			}

			ImGui::SameLine();
			if (ImGui::Button("Search##SearchClickImage"))
			{

			}

			ImGui::EndChild();
			ImGui::PopStyleColor();
		}

		ImGui::EndChild();
		ImGui::PopStyleColor();

		// Actions Callback
		ImGui::PushFont(App->moduleImGui->rudaBlackBig);
		ImGui::Text("Actions when button is clicked:");
		ImGui::PopFont();

		ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0.12f, 0.14f, 0.17f, 1.00f));
		ImGui::BeginChild("ButtonActions", ImVec2(ImGui::GetContentRegionAvailWidth(), 350)); 

		INC_CURSOR_10;
		if (ImGui::Button("Add On Click Action"))
		{

		}

		INC_CURSOR_10;
		ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0.082f, 0.105f, 0.13f, 1.0f));
		ImGui::BeginChild("ButtonActions", ImVec2(ImGui::GetContentRegionAvailWidth() - 10, 300));



		ImGui::EndChild();
		ImGui::PopStyleColor();

		ImGui::EndChild();
		ImGui::PopStyleColor(); 
	}

}

void ObjectPropertiesDockPanel::DrawColorTintSection()
{
	IMGUI_SPACED_SEPARATOR;

	static float mouseOverColor[4] = { 0,0,0,0 };
	INC_CURSOR_X_10
		ImGui::ColorEdit4("Mouse Over Tint", mouseOverColor);

	static float mouseClickTint[4] = { 0,0,0,0 };
	INC_CURSOR_X_10
		ImGui::ColorEdit4("Mouse Click Tint", mouseClickTint);
}

void ObjectPropertiesDockPanel::DrawButtonMainImagePreview(UI_Button* selectedButton)
{
	ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0.12f, 0.14f, 0.17f, 1.00f));
	ImGui::BeginChild("ButtonImagePreview", ImVec2(ImGui::GetContentRegionAvailWidth(), 195));

	ImVec2 imageMaxSize = ImVec2(ImGui::GetContentRegionAvailWidth(), 135);
	ImVec2 uiImageDimensions = ImVec2(150, 150);
	Texture* mainButtonTexture = selectedButton->GetMainTexture();
	ImTextureID mainTextureID = 0;

	if (mainButtonTexture != nullptr)
	{
		float aspectRatio = mainButtonTexture->GetAspectRatio();

		if (mainButtonTexture->IsVertical())
		{
			uiImageDimensions.y = imageMaxSize.y;
			uiImageDimensions.x = uiImageDimensions.x * aspectRatio;
		}
		else
		{
			uiImageDimensions.y = imageMaxSize.y;
			uiImageDimensions.x = uiImageDimensions.y * aspectRatio;

			if (uiImageDimensions.x > imageMaxSize.x)
			{
				float diff = uiImageDimensions.x - imageMaxSize.x;
				uiImageDimensions.x -= diff;
				uiImageDimensions.y = uiImageDimensions.x * aspectRatio;
			}
		}
		mainTextureID = (ImTextureID)mainButtonTexture->GetTextureID();
	}

	ImGui::Spacing();

	ImGui::SetCursorPos(ImVec2(ImGui::GetContentRegionAvailWidth() / 2 - (uiImageDimensions.x / 2), imageMaxSize.y / 2 - (uiImageDimensions.y / 2) + 10));
	ImGui::Image(mainTextureID, uiImageDimensions);

	ImGui::Spacing();

	static char searchButtonImageBuffer[256];

	ImGui::Spacing();
	ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 5);
	ImGui::SetNextItemWidth(ImGui::GetContentRegionAvailWidth() - 70);
	ImGui::InputTextWithHint("", "Search Image...", searchButtonImageBuffer, IM_ARRAYSIZE(searchButtonImageBuffer));

	if (ImGui::BeginDragDropTarget())
	{
		if (const ImGuiPayload * payload = ImGui::AcceptDragDropPayload("drag_resource"))
		{
			int* selectedResourceUID = (int*)payload->Data;
			Resource* resourceDropped = ResourceManager::getInstance()->GetResource(*selectedResourceUID);

			if (resourceDropped->GetType() == RESOURCE_TEXTURE)
			{
				Texture* textureDropped = (Texture*)resourceDropped;
				selectedButton->SetMainTexture(textureDropped);
			}
		}

		ImGui::EndDragDropTarget();
	}

	ImGui::SameLine();
	if (ImGui::Button("Search"))
	{

	}

	ImGui::EndChild();
	ImGui::PopStyleColor();
}

void ObjectPropertiesDockPanel::DrawUIImageProperties(UI_Element* selectedUIElement)
{
	UI_Image* selectedImage = (UI_Image*)selectedUIElement;
	static char uiImageNameBuffer[256] = "";

	if (selectedImage->GetDisplayImage()->GetTexture() != nullptr)
		strcpy(uiImageNameBuffer, selectedImage->GetDisplayImage()->GetTexture()->GetName().c_str());

	if (ImGui::CollapsingHeader("UI Image Properties", ImGuiTreeNodeFlags_DefaultOpen))
	{
		ImGui::Spacing();

		ImGui::InputTextWithHint("", "Search...", uiImageNameBuffer, IM_ARRAYSIZE(uiImageNameBuffer));

		ImGui::SameLine();
		if (ImGui::Button("Search##SearchUIImage"))
		{

		}

		ImGui::Spacing();

		PUSH_CHILD_BG_COLOR;
		ImGui::BeginChild("PreviewUIImageLeftCol", ImVec2(ImGui::GetContentRegionAvail().x, 200));

		ImVec2 centerPoint = ImVec2(ImGui::GetContentRegionAvail().x / 2, ImGui::GetContentRegionAvail().y / 2);

		ImGui::EndChild();
		ImGui::PopStyleColor();

		PUSH_CHILD_BG_COLOR;
		ImGui::BeginChild("PreviewUIImageRightCol", ImVec2(ImGui::GetContentRegionAvail().x, 100));

		if (selectedImage != nullptr)
		{
			ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 5); 
			ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 3); 
			ImGui::PushFont(App->moduleImGui->rudaBoldBig); 
			ImGui::Text("Name:"); 
			ImGui::PopFont();

			ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 5); 
			ImGui::PushFont(App->moduleImGui->rudaRegularSmall);
			ImGui::Text("%s", selectedImage->GetDisplayImage()->GetTexture()->GetName().c_str());
			ImGui::PopFont();
		}


		ImGui::EndChild();
		ImGui::PopStyleColor();
	}
}

void ObjectPropertiesDockPanel::DrawFlyObjectProperties()
{
	FlyObject* selectedObject = App->moduleRoomManager->GetSelectedRoom()->GetSelectedObject();

	if (selectedObject != nullptr)
	{
		DrawFixedPartObjectUI(selectedObject);

		ImGui::Separator();

		ImGui::PushFont(App->moduleImGui->rudaBoldHuge);
		ImGui::Text("Item Inventory Attributes:");
		ImGui::PopFont();

		if (ImGui::BeginTabBar("MyTabBar", ImGuiTabBarFlags_None))
		{
			if (selectedObject->IsInventoryItem())
			{

				if (ImGui::BeginTabItem("Item Settings"))
				{
					static char inventoryBrowcseImageBuffer[512];
					ImGui::InputTextWithHint("", "Search...", inventoryBrowcseImageBuffer, IM_ARRAYSIZE(inventoryBrowcseImageBuffer));
					ImGui::SameLine();
					if (ImGui::Button("Change Image##Properties"))
					{
						ImGui::Columns(2);

						ImGui::Text("Position");
						ImGui::DragFloat("X", &selectedObject->GetClickableAreaPosOne().x);
						ImGui::DragFloat("Y", &selectedObject->GetClickableAreaPosOne().y);

						ImGui::NextColumn();

						ImGui::Text("Dimensions");
						ImGui::DragFloat("Width", &selectedObject->GetClickableAreaSizeOne().x);
						ImGui::DragFloat("Heigth", &selectedObject->GetClickableAreaSizeOne().y);
						ImGui::EndChild();
					}

					ImGui::EndTabItem();
				}
			}
			else
			{
				if (ImGui::BeginTabItem("Actions"))
				{
					DrawObjectActionsTab();
					ImGui::EndTabItem();
				}

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

void ObjectPropertiesDockPanel::DrawFixedPartObjectUI(FlyObject* selectedObject)
{
	// Draw Top Manage Buttons ----------
	DrawTransformButtons();

	ImGui::Separator();
	ImGui::Separator();

	Texture* objectIconTexture = nullptr;
	if(selectedObject->flyObjectType == ACTION_OBJECT)
		objectIconTexture = (Texture*)ResourceManager::getInstance()->GetResource("ObjectIcon");
	else if (selectedObject->flyObjectType == INVENTORY_ITEM)
		objectIconTexture = (Texture*)ResourceManager::getInstance()->GetResource("InventoryItemIcon");
	else if (selectedObject->flyObjectType == UI_HOLDER)
		objectIconTexture = (Texture*)ResourceManager::getInstance()->GetResource("UserInterfaceIcon");

	if(objectIconTexture != nullptr)
		ImGui::Image((ImTextureID)objectIconTexture->GetTextureID(), ImVec2(35, 35));

	ImGui::Separator();

	ImGui::SetCursorPos(ImVec2(ImGui::GetCursorPosX() + 45, ImGui::GetCursorPosY() - 38));

	ImGui::PushFont(App->moduleImGui->rudaBoldHuge);
	ImGui::Text("%s", selectedObject->GetName().c_str());
	ImGui::PopFont();

	if (!selectedObject->GetDescription().empty()) 
	{
		ImGui::Spacing();	
		ImGui::TextColored(ImVec4(0.8f, 0.8f, 1.0f, 1.0f), "%s", selectedObject->GetDescription().c_str());
	}

	ImGui::Spacing();
	DrawObjectPlacementCH(selectedObject);
	ImGui::Spacing();
}

void ObjectPropertiesDockPanel::DrawTransformButtons()
{
	bool currentMode = false;

	// Select Gizmo ----------------
	if (App->moduleImGui->gameViewportDockPanel->GetGizmoMode() == GIZMO_SELECT) {
		currentMode = true;
		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.4f, 0.7f, 1, 0.35f));
	}

	Texture* arrowSelect = (Texture*)ResourceManager::getInstance()->GetResource("SelectArrow");
	if (ImGui::ImageButton((ImTextureID)arrowSelect->GetTextureID(), ImVec2(22, 22)))
	{
		App->moduleImGui->gameViewportDockPanel->SetGizmoMode(GIZMO_SELECT); 

		FlyObject* selectedObject = App->moduleRoomManager->GetSelectedRoom()->GetSelectedObject();
		if (selectedObject != nullptr)
		{
			selectedObject->CalculateCurrentGizmo();
		}
	}

	if (currentMode) {
		currentMode = false;
		ImGui::PopStyleColor();
	}

	// Move Gizmo ----------------
	currentMode = false;
	if (App->moduleImGui->gameViewportDockPanel->GetGizmoMode() == GIZMO_MOVE) {
		currentMode = true;
		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.4f, 0.7f, 1, 0.35f));
	}

	ImGui::SameLine();
	Texture* moveArrows = (Texture*)ResourceManager::getInstance()->GetResource("MoveOption");
	if (ImGui::ImageButton((ImTextureID)moveArrows->GetTextureID(), ImVec2(22, 22)))
	{
		App->moduleImGui->gameViewportDockPanel->SetGizmoMode(GIZMO_MOVE);
		FlyObject* selectedObject = App->moduleRoomManager->GetSelectedRoom()->GetSelectedObject();
		if (selectedObject != nullptr)
		{
			selectedObject->CalculateCurrentGizmo();
		}
	}

	if (currentMode) {
		currentMode = false;
		ImGui::PopStyleColor();
	}

	// Scale Gizmo ----------------
	currentMode = false;
	if (App->moduleImGui->gameViewportDockPanel->GetGizmoMode() == GIZMO_SCALE) {
		currentMode = true;
		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.4f, 0.7f, 1, 0.35f));
	}

	ImGui::SameLine();
	Texture* scaleIcon = (Texture*)ResourceManager::getInstance()->GetResource("ScaleIcon");
	if (ImGui::ImageButton((ImTextureID)scaleIcon->GetTextureID(), ImVec2(22, 22)))
	{
		App->moduleImGui->gameViewportDockPanel->SetGizmoMode(GIZMO_SCALE);
		FlyObject* selectedObject = App->moduleRoomManager->GetSelectedRoom()->GetSelectedObject();
		if (selectedObject != nullptr)
		{
			selectedObject->CalculateCurrentGizmo();
		}
	}

	if (currentMode) {
		currentMode = false;
		ImGui::PopStyleColor();
	}
}

void ObjectPropertiesDockPanel::DrawObjectActionsTab()
{
	// Draw Objects List ---------
	DrawActionsList();

	ImGui::Spacing();
	ImGui::Separator();

	// Draw Tool --------
	DrawActionSettings();
}

void ObjectPropertiesDockPanel::DrawClickableAreaTab()
{
	ImGui::Spacing();

	ImGui::Checkbox("Draw##DrawCA", &selectedObject->drawClickableArea); ImGui::SameLine();
	ImGui::Checkbox("Active##ActiveCA", &selectedObject->clickableAreaActive); 

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
		//selectedObject->GetClickableArea()->drawCornerPoints = true; 
	}
}

void ObjectPropertiesDockPanel::DrawActionSettings()
{
	if (selectedObject->GetSelectedActionType() != AT_null)
	{
		switch (selectedObject->GetSelectedActionType())
		{
		case AT_DISPLAY_IMAGE:
			DrawToolImageSettings();
			break;

		case AT_CHANGE_ROOM:
			DrawChangeRoomSettings();
			break;

		case AT_MOD_VARIABLE:
			DrawModifyVariableSettings();
			break;

		case AT_EMIT_SOUND:
			DrawEmitSoundSettings();
			break;
		}
	}
}

void ObjectPropertiesDockPanel::DrawModifyVariableSettings()
{
	ModifyVariableAction* modifyVariableAction = (ModifyVariableAction*)selectedObject->GetAction("Modify Variable");

	if (modifyVariableAction != nullptr)
	{
		if (ImGui::CollapsingHeader("Modify Variable Attributes", ImGuiTreeNodeFlags_DefaultOpen))
		{
			// Object Occurrence ---------
			ImGui::PushFont(App->moduleImGui->rudaBoldBig);
			ImGui::Text("Action Happens On:");
			ImGui::PopFont();

			ImGui::PushFont(App->moduleImGui->rudaRegularMid);
			ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0.12f, 0.14f, 0.17f, 1.00f));

			ImGui::BeginChild("##OccChild", ImVec2(ImGui::GetContentRegionAvailWidth(), 70));

			ImGui::SetCursorPos(ImVec2(5, 8));
			ImGui::Checkbox("Object Clicked", &modifyVariableAction->IsOccObjectClicked());
			ImGui::SetCursorPos(ImVec2(5, 38));
			ImGui::Checkbox("Blackboard Value Condition", &modifyVariableAction->IsOccBlackboardValue());

			ImGui::SameLine();
			static std::string showValueConditionButtonText = "Show Conditions"; 
			if (ImGui::Button(showValueConditionButtonText.c_str()))
			{
				if (showVariableConditions)
				{
					showVariableConditions = false;
					showValueConditionButtonText = "Show Conditions";
				}
				else
				{
					showVariableConditions = true;
					showValueConditionButtonText = "Hide Conditions";
				}
			}

			ImGui::EndChild();

			if (showVariableConditions)
				modifyVariableAction->DrawValueConditionsList();

			POP_FONT;
			ImGui::PopStyleColor();

			IMGUI_SPACED_SEPARATOR;

			// Object Settings ----------
			ImGui::PushFont(App->moduleImGui->rudaBoldBig);
			ImGui::Text("Variables To Modify:");
			ImGui::PopFont();

			modifyVariableAction->DrawEffectVariablesUI();

			Texture* plusIcon = (Texture*)ResourceManager::getInstance()->GetResource("PlusIcon");
			if (ImGui::ImageButton((ImTextureID)plusIcon->GetTextureID(), ImVec2(30, 30)))
			{
				modifyVariableAction->AddEmptyEffect();
			}

			ImGui::SameLine();
			Texture* minusIcon = (Texture*)ResourceManager::getInstance()->GetResource("MinusIcon");
			if (ImGui::ImageButton((ImTextureID)minusIcon->GetTextureID(), ImVec2(30, 30)))
			{

			}
		}
	}
}

void ObjectPropertiesDockPanel::DrawEmitSoundSettings()
{
	EmitSoundAction* emitSoundAction = (EmitSoundAction*)selectedObject->GetAction("Emit Sound");

	if (emitSoundAction != nullptr)
	{
		if (ImGui::CollapsingHeader("Emit Sound Attributes", ImGuiTreeNodeFlags_DefaultOpen))
		{
			ImGui::PushFont(App->moduleImGui->rudaBoldBig);
			ImGui::Text("Action Happens On:");
			ImGui::PopFont();

			ImGui::PushFont(App->moduleImGui->rudaRegularMid);
			ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0.12f, 0.14f, 0.17f, 1.00f));
			ImGui::BeginChild("##OccChild", ImVec2(ImGui::GetContentRegionAvailWidth(), 70));

			ImGui::SetCursorPos(ImVec2(5, 8));
			ImGui::Checkbox("Object Clicked", &emitSoundAction->IsOccObjectClicked());
			ImGui::SetCursorPos(ImVec2(5, 38));
			ImGui::Checkbox("Blackboard Value Condition", &emitSoundAction->IsOccBlackboardValue());

			ImGui::SameLine();
			static std::string showValueConditionButtonText = "Show Conditions";
			if (ImGui::Button(showValueConditionButtonText.c_str()))
			{
				if (showVariableConditions)
				{
					showVariableConditions = false;
					showValueConditionButtonText = "Show Conditions";
				}
				else
				{
					showVariableConditions = true;
					showValueConditionButtonText = "Hide Conditions";
				}
			}

			if (showVariableConditions)
				emitSoundAction->DrawValueConditionsList();

			ImGui::Spacing();
			ImGui::EndChild();

			ImGui::PopFont();
			ImGui::PopStyleColor();

			ImGui::PushFont(App->moduleImGui->rudaBlackBig);
			ImGui::Text("Sound To Play:"); 
			ImGui::PopFont(); 

			static char soundNameBuffer[256] = "";

			if (emitSoundAction->audioClip != nullptr)
			{
				strcpy(soundNameBuffer, emitSoundAction->audioClip->GetName().c_str());
			}

			Texture* playSound = (Texture*)ResourceManager::getInstance()->GetResource("PlayAudio");
			if (ImGui::ImageButton((ImTextureID)playSound->GetTextureID(), ImVec2(20, 20)))
			{
				emitSoundAction->Play(); 
			}

			ImGui::SameLine();

			ImGui::InputTextWithHint("", "Select Sound...", soundNameBuffer, IM_ARRAYSIZE(soundNameBuffer), ImGuiInputTextFlags_ReadOnly);

			if (ImGui::BeginDragDropTarget())
			{
				if (const ImGuiPayload * payload = ImGui::AcceptDragDropPayload("drag_resource"))
				{
					int* selectedResourceUID = (int*)payload->Data;
					Resource* resourceDropped = ResourceManager::getInstance()->GetResource(*selectedResourceUID);

					if (resourceDropped->GetType() == RESOURCE_SFX)
					{
						AudioClip* audioClipDropped = (AudioClip*)resourceDropped;
						emitSoundAction->audioClip = audioClipDropped;				
					}
				}

				ImGui::EndDragDropTarget();
			}

			ImGui::SameLine();
			if (ImGui::Button("Search##SearchSound"))
			{
				ImGui::OpenPopup("print_sound_selection_popup");
				showSoundSelectionPopup = true;
			}

			if (showSoundSelectionPopup)
			{
				Resource* selectedSound = ResourceManager::getInstance()->PrintSoundsSelectionPopup();

				if (selectedSound != nullptr)
				{
					AudioClip* audioClipDropped = (AudioClip*)selectedSound;
					emitSoundAction->audioClip = audioClipDropped;
					showSoundSelectionPopup = false;
				}
			}
		}
	}
}

void ObjectPropertiesDockPanel::DrawChangeRoomSettings()
{
	ChangeRoomAction* changeRoomAction = (ChangeRoomAction*)selectedObject->GetAction("Change Room");

	if (changeRoomAction != nullptr)
	{
		if (ImGui::CollapsingHeader("Change Room Attributes", ImGuiTreeNodeFlags_DefaultOpen))
		{
			ImGui::PushFont(App->moduleImGui->rudaBoldBig);
			ImGui::Text("Action Happens On:");
			ImGui::PopFont();

			ImGui::PushFont(App->moduleImGui->rudaRegularMid);
			ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0.12f, 0.14f, 0.17f, 1.00f));
			ImGui::BeginChild("##OccChild", ImVec2(ImGui::GetContentRegionAvailWidth(), 35));

			ImGui::SetCursorPos(ImVec2(5, 5));
			ImGui::Checkbox("Object Clicked", &changeRoomAction->IsOccObjectClicked());

			ImGui::SameLine();
			if (ImGui::SmallButton("Adjust Clickable Area"))
			{
				
			}

			ImGui::Spacing();
			ImGui::EndChild();

			ImGui::PopFont();
			ImGui::PopStyleColor();

			IMGUI_SPACED_SEPARATOR;

			ImGui::PushFont(App->moduleImGui->rudaBoldBig);
			ImGui::Text("Change Room Settings: ");
			ImGui::PopFont();

			const char** rooms = App->moduleRoomManager->GetRoomsAsCombo();
			const char* roomsToCombo[] = { "None", *rooms, *rooms + 1, *rooms + 2};
			static int ci = 0;
			ImGui::ComboArray("Destination", &ci, roomsToCombo, IM_ARRAYSIZE(roomsToCombo));
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
		DrawActionSelectable(selectableInfo, currentTool, count, 40);
		count++; 
	}

	ImGui::EndChild();

	ImGui::PopStyleVar();
	ImGui::PopStyleColor();

	DrawAddAndDeleteButtons();
}

void ObjectPropertiesDockPanel::DrawAddAndDeleteButtons()
{
	ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(2, 2));

	Texture* plusIconTex = (Texture*)ResourceManager::getInstance()->GetResource("PlusIconWhite");
	if (ImGui::ImageButton((ImTextureID)plusIconTex->GetTextureID(), ImVec2(25, 25)))
	{
		showToolDictionary = true; 
	}

	ImGui::SameLine();
	Texture* minusIconTex = (Texture*)ResourceManager::getInstance()->GetResource("MinusIconWhite");
	if (ImGui::ImageButton((ImTextureID)minusIconTex->GetTextureID(), ImVec2(25, 25)))
	{	
		Action* selectedAction = selectedObject->selectedAction; 

		if (selectedAction != nullptr)
		{
			selectedAction->CleanUp();
			selectedObject->DeleteAction(selectedAction->GetActionName());
			selectedAction = nullptr;
		}
	}

	ImGui::PopStyleVar();

	if (showToolDictionary)
	{
		ImGui::Spacing();
		ImGui::BeginChild("AddToolObjectProperties", ImVec2(ImGui::GetContentRegionAvailWidth(), 200)); 

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
			case AT_DISPLAY_IMAGE:
				selectedObject->AddDisplayImageAction("None");
				break;

			case AT_CHANGE_ROOM:
				selectedObject->AddChangeRoomAction();
				break;

			case AT_MOD_VARIABLE:
				selectedObject->AddModifyVariableAction();
				break;

			case AT_EMIT_SOUND:
				selectedObject->AddEmitSoundAction();
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

void ObjectPropertiesDockPanel::DrawActionSelectable(ActionSelectableInfo& selectableInfo, Action*& currentAction, int posInList, int selectableHeigth)
{
	ImGui::PushFont(App->moduleImGui->rudaBoldMid);

	Texture* imageIcon = App->moduleManager->GetIconFromActionType(selectableInfo.actionType); 
	ImGui::SetCursorPos(ImVec2(10, 7 + (selectableHeigth * posInList)));
	ImGui::Image((ImTextureID)imageIcon->GetTextureID(), ImVec2(30, 30), ImVec2(0, 1), ImVec2(1, 0));

	ImGui::SetCursorPos(ImVec2(50, ((selectableHeigth + 5) * posInList)));
	if (ImGui::Selectable(selectableInfo.actionName.c_str(), currentAction->IsSelected(), ImGuiSelectableFlags_None, ImVec2(ImGui::GetContentRegionAvailWidth(), selectableHeigth))) {
		selectedObject->SetSelectedAction(selectableInfo.actionType);
	}
	ImGui::PopFont();

	// Description -----
	ImGui::SetCursorPosY(((selectableHeigth + 5) * posInList) + 24);
	ImGui::SetCursorPosX(ImGui::GetCursorPos().x + 52);

	ImGui::PushFont(App->moduleImGui->rudaRegularTiny);
	ImGui::Text(selectableInfo.actionDescription.c_str());
	ImGui::PopFont();
}

void ObjectPropertiesDockPanel::DrawObjectPlacementCH(FlyObject* selectedObject)
{
	ImGui::PushFont(App->moduleImGui->rudaBoldMid);
	if (ImGui::CollapsingHeader("Object Placement", ImGuiTreeNodeFlags_DefaultOpen))
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
	DisplayImageAction* imageTool = (DisplayImageAction*)selectedObject->GetAction("Display Image");

	if (imageTool != nullptr)
	{
		if (ImGui::CollapsingHeader("Image Tool Settings"))
		{
			static char buf[256] = "";

			Texture* imageToolTexture = imageTool->GetTexture(); 

			if (imageToolTexture == nullptr)
				imageToolTexture = (Texture*)ResourceManager::getInstance()->GetResource("EmptyObject");

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
					flog("Player Opened %s", path);
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

