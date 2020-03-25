#include "ObjectCreatorDockPanel.h"
#include "DisplayAnimationAction.h"
#include "Application.h"
#include "TinyFileDialog.h"
#include "ModuleImGui.h"
#include "Action.h"
#include "ModuleRoomManager.h"
#include "MyFileSystem.h"
#include "ChangeRoomAction.h"
#include "FlyVariable.h"
#include "EmitSoundAction.h"
#include "Room.h"
#include "ModifyVariableAction.h"
#include "DisplayImageAction.h"
#include "Texture.h"
#include "FollowPathAction.h"
#include "ImageImporter.h"
#include "DisplayTextAction.h"
#include "ViewportManager.h"
#include "AudioClip.h"
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
		Texture* iconTexture = nullptr; 

		if (creatingObject->flyObjectType == FlyObjectType::ACTION_OBJECT)	
			iconTexture = (Texture*)ResourceManager::getInstance()->GetResource("ObjectIcon");	

		else if (creatingObject->flyObjectType == FlyObjectType::INVENTORY_ITEM)		
			iconTexture = (Texture*)ResourceManager::getInstance()->GetResource("InventoryItemIcon");

		else if (creatingObject->flyObjectType == FlyObjectType::OBJECT_SEQUENTIAL)
			iconTexture = (Texture*)ResourceManager::getInstance()->GetResource("InventoryItemIcon");

		ImGui::Image((ImTextureID)iconTexture->GetTextureID(), ImVec2(35, 35)); 
		ImGui::SameLine(); 

		INC_CURSOR_7;
		ImGui::PushFont(App->moduleImGui->rudaBlackHuge);
		ImGui::Text("Object Creator:");
		ImGui::PopFont();

		ImGui::SameLine();

		if (DrawCloseAndCreateButton())
		{
			ImGui::End();
			return true; 
		}

		ImGui::Separator();
		ImGui::Spacing();

		ImGui::PushFont(App->moduleImGui->rudaRegularMid);
		ImGui::InputTextWithHint("Name##ObjectNaming", "Name...", newObjectName, 256 * sizeof(char));
		ImGui::PopFont();

		ImGui::InputTextMultiline("Description##ObjectDescription", newObjectDescription, 256 * sizeof(char), ImVec2(ImGui::GetContentRegionMax().x - 100, 100));

		static int selectedObjectType = 0;
		if (ImGui::Combo("Object Type", &selectedObjectType, "Action Object\0Inventory Item\0Sequential Object\0"))
		{
			switch (selectedObjectType)
			{
			case ACTION_OBJECT: 
				ResetObjectData();
				creatingObject->flyObjectType = ACTION_OBJECT;
				break; 

			case INVENTORY_ITEM: 
				ResetObjectData();
				clickableAreaActive = true; 
				creatingObject->flyObjectType = INVENTORY_ITEM; 
				break;

			case OBJECT_SEQUENTIAL:
				ResetObjectData();
				clickableAreaActive = true;
				creatingObject->flyObjectType = OBJECT_SEQUENTIAL;
				break;
			}
		}

		if (ImGui::Checkbox("Interactable", &creatingObject->IsInteractable()))
		{
			focusClickableAreaTab = true;
		}

		ImGui::Separator(); 

		if (selectedObjectType == ACTION_OBJECT || selectedObjectType == OBJECT_SEQUENTIAL)
		{
			if (selectedObjectType == ACTION_OBJECT)
				DrawObjectCreator();
			else
				DrawObjectSequentialCreator();
			
			if (creatingObject->IsInteractable())
			{
				ImGuiTabItemFlags tabFlags = 0;
				if (focusClickableAreaTab)
				{
					tabFlags |= ImGuiTabItemFlags_SetSelected;
					focusClickableAreaTab = false;
				}

				DrawClickableAreaCreator();
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
void ObjectCreatorDockPanel::DrawObjectSequentialCreator()
{
	static bool toSequentialList = false; 
	static bool updatePopup = false; 
	ImGui::PushFont(App->moduleImGui->rudaBlackBig);
	ImGui::Text("Fixed Actions:");
	ImGui::PopFont();

	DrawObjectActionsList();
	
	ImGui::Spacing();
	ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(2, 2));

	ImGui::SetCursorPosY(ImGui::GetCursorPosY() - 5);
	Texture* plusIconTex = (Texture*)ResourceManager::getInstance()->GetResource("PlusIconWhite");

	if (ImGui::ImageButton((ImTextureID)plusIconTex->GetTextureID(), ImVec2(30, 30)))
	{
		flog("Plus Fixed");
		toSequentialList = false;
		ImGui::OpenPopup("AddActionToObject");
	}

	ImGui::SameLine();
	Texture* minusIconTex = (Texture*)ResourceManager::getInstance()->GetResource("MinusIconWhite");
	if (ImGui::ImageButton((ImTextureID)minusIconTex->GetTextureID(), ImVec2(30, 30)))
	{
		if (selectedAction != nullptr)
		{
			creatingObject->DeleteAction(selectedAction->GetType());
			selectedAction = nullptr;
		}
	}

	ImGui::PopStyleVar();

	ImGui::Spacing(); 
	ImGui::Separator(); 

	ImGui::PushFont(App->moduleImGui->rudaBlackBig);
	ImGui::Text("Sequential Actions:");
	ImGui::PopFont();

	DrawSequentialActionsList();
	
	ImGui::Spacing();
	ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(2, 2));

	ImGui::SetCursorPosY(ImGui::GetCursorPosY() - 5);
	plusIconTex = (Texture*)ResourceManager::getInstance()->GetResource("PlusIconWhite2");

	if (ImGui::ImageButton((ImTextureID)plusIconTex->GetTextureID(), ImVec2(30, 30)))
	{
		flog("Sequential Fixed");
		toSequentialList = true; 
		ImGui::OpenPopup("AddActionToObject");
	}

	ImGui::SameLine();
	minusIconTex = (Texture*)ResourceManager::getInstance()->GetResource("MinusIconWhite");
	if (ImGui::ImageButton((ImTextureID)minusIconTex->GetTextureID(), ImVec2(30, 30)))
	{
		if (selectedAction != nullptr)
		{
			creatingObject->DeleteAction(selectedAction->GetType());
			selectedAction = nullptr;
		}
	}

	ImGui::SameLine();
	Texture* arrowIcon = (Texture*)ResourceManager::getInstance()->GetResource("ArrowDownWhite");
	if (ImGui::ImageButton((ImTextureID)arrowIcon->GetTextureID(), ImVec2(30, 30)))
	{
	}

	ImGui::SameLine();
	arrowIcon = (Texture*)ResourceManager::getInstance()->GetResource("ArrowUpWhite");
	if (ImGui::ImageButton((ImTextureID)arrowIcon->GetTextureID(), ImVec2(30, 30)))
	{
	}

	ImGui::PopStyleVar();

	if (ImGui::BeginTabBar("SelectedSettings"))
	{
		DrawSelectedActionSettings();
		ImGui::EndTabBar();
	}

	// Callbacks for buttons 
	if (ImGui::BeginPopup("AddActionToObject"))
	{
		ImGui::Spacing();

		// Search Bar ---------------
		ImGui::InputTextWithHint("##SearchTool", "Search...", searchNewActionBuffer, IM_ARRAYSIZE(searchNewActionBuffer));
		ImGui::SameLine();

		Texture* filterIcon = (Texture*)ResourceManager::getInstance()->GetResource("FilterIcon");
		ImGui::Image((ImTextureID)filterIcon->GetTextureID(), ImVec2(22, 22));

		ImGui::Spacing();

		ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0.12f, 0.14f, 0.17f, 1.00f));
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(2.0f, 2.0f));
		ImGui::BeginChild("##4ShowImage", ImVec2(ImGui::GetContentRegionAvailWidth(), 150));

		// Tools Dictonary ----------
		ActionSelectableInfo* newActionSelected = nullptr;

		if(toSequentialList)
			newActionSelected = App->moduleManager->DrawActionDictionaryUI();
		else
			newActionSelected = App->moduleManager->DrawActionDictionaryUI(FILTER_ACTIONS_FIXED);

		if (newActionSelected != nullptr)
		{
			switch (newActionSelected->actionType)
			{
			case ACTION_DISPLAY_IMAGE:
				selectedAction = creatingObject->AddDisplayImageAction(std::string(MyFileSystem::getInstance()->GetIconsDirectory() + "EmptyObject.png").c_str(), toSequentialList);
				break;

			case ACTION_CHANGE_ROOM:
				selectedAction = creatingObject->AddChangeRoomAction(toSequentialList);
				break;

			case ACTION_MOD_VARIABLE:
				selectedAction = creatingObject->AddModifyVariableAction(toSequentialList);
				break;

			case ACTION_EMIT_SOUND:
				selectedAction = creatingObject->AddEmitSoundAction(toSequentialList);
				break;

			case ACTION_DISPLAY_TEXT:
				selectedAction = creatingObject->AddDisplayTextAction(toSequentialList);
				break;

			case ACTION_DISPLAY_ANIMATION:
				selectedAction = creatingObject->AddDisplayAnimationAction(toSequentialList);
				break;

			case ACTION_FOLLOW_PATH:
				selectedAction = creatingObject->AddFollowPathAction(toSequentialList);
				break;

			case AT_null:
				break;
			}

			if (toSequentialList && selectedAction != nullptr)
				selectedAction->SetActionClass(ACTION_CLASS_SEQUENTIAL); 

			ImGui::EndChild();
			ImGui::PopStyleColor();
			ImGui::PopStyleVar();
			ImGui::CloseCurrentPopup();
			ImGui::EndPopup();
			return;
		}

		ImGui::EndChild();
		ImGui::PopStyleColor();
		ImGui::PopStyleVar();

		ImGui::EndPopup();
	}
}

void ObjectCreatorDockPanel::DrawInventorySettings()
{
	// ----------------------------------------------------------------------------------------
	// Draw the preview of the object in order to adjust the clickable area 
	// ----------------------------------------------------------------------------------------
	SPACING;
	PrintClickableAreaObjectVisuals(drawClickableAreaOver);
	SPACING; 

	// Input Text to Browse Image ------------
	static char inventoryBrowcseImageBuffer[512];
	ImGui::InputTextWithHint("", "Search...", inventoryBrowcseImageBuffer, IM_ARRAYSIZE(inventoryBrowcseImageBuffer));
	ImGui::SameLine();

	if (ImGui::BeginDragDropTarget())
	{
		if (const ImGuiPayload * payload = ImGui::AcceptDragDropPayload("drag_resource"))
		{
			int* selectedResourceUID = (int*)payload->Data;
			Resource* resourceDropped = ResourceManager::getInstance()->GetResource(*selectedResourceUID);

			if (resourceDropped->GetType() == RESOURCE_TEXTURE)
			{
				Texture* textureDropped = (Texture*)resourceDropped;
	
				if (displayImageAction_Inv == nullptr)				
					displayImageAction_Inv = creatingObject->AddDisplayImageAction(textureDropped->GetPath());				
				else				
					displayImageAction_Inv->SetTexture(textureDropped);
				
				strcpy(inventoryBrowcseImageBuffer, resourceDropped->GetName().c_str());
			}
		}

		ImGui::EndDragDropTarget();
	}

	if (ImGui::Button("Change Image##Creator"))
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

	// ----------------------------------------------------------------------------------------
	// Draw Clickable Zone Settings  
	// ----------------------------------------------------------------------------------------
	ImGui::PushFont(App->moduleImGui->rudaBlackBig);
	ImGui::Text("Clickable Zone:");
	ImGui::PopFont();

	ImGui::Separator(); 
	ImGui::Spacing();

	if (displayImageAction_Inv != nullptr && displayImageAction_Inv->GetTexture() != nullptr)
	{
		ImGui::Checkbox("Draw", &drawClickableAreaOver);

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
		ImGui::PushFont(App->moduleImGui->rudaRegularTiny);
		ImGui::TextColored(ImVec4(0,1,1,1), "Inventory Items need images assigned to enable the clickable zone. Assign an image.");
		ImGui::PopFont();
	}

	// ----------------------------------------------------------------------------------------
	// Select The Action that the player wants to trigger when the object is picked up 
	// ----------------------------------------------------------------------------------------
	ImGui::Separator();

	ImGui::PushFont(App->moduleImGui->rudaBlackBig);
	ImGui::Text("Perform Action When Picked:");
	ImGui::PopFont();

	ImGui::SameLine(); 

	if (ImGui::Button("Add On Pick Action"))
	{
		ImGui::OpenPopup("selectOnClickAction"); 
	}
	
	if (ImGui::BeginPopup("selectOnClickAction"))
	{
		ActionSelectableInfo* selectableInfo = App->moduleManager->DrawActionDictionaryUI(FILTER_ACTIONS_INVENTORY_CLICK);
		Action* addedAction = nullptr; 

		if (selectableInfo != nullptr)
		{
			switch (selectableInfo->actionType)
			{
			case ACTION_CHANGE_ROOM:
				addedAction = creatingObject->AddChangeRoomAction();
				break;

			case ACTION_MOD_VARIABLE:
				addedAction = creatingObject->AddModifyVariableAction();
				break;

			case ACTION_EMIT_SOUND:
				addedAction = creatingObject->AddEmitSoundAction();
				break;
			}
		}

		if (addedAction != nullptr)
			addedAction->SetOccObjectClicked(true); 
		
		ImGui::EndPopup(); 
	}

	// ----------------------------------------------------------------------------------------
	// Draw the list of the action that will trigger when the item is clicked or picked up
	// ----------------------------------------------------------------------------------------
	PUSH_CHILD_BG_COLOR; 
	ImGui::BeginChild("OnPickActions", ImVec2(ImGui::GetContentRegionMax().x, 150)); 

	int count = 0; 
	bool setScroll = false; 
	for (auto& currentAction : creatingObject->GetActionsList())
	{
		// In Inventory Items, the image is fixed and we dont want to treat it as an action 
		if (currentAction->GetActionName() == "Display Image")
			continue; 

		if (DrawSelectable(currentAction->GetActionSelectableInfo(), currentAction->IsSelected(), count, 42, currentAction))		
			setScroll = true; 
		
		count++; 
	}

	ImGui::EndChild(); 
	ImGui::PopStyleColor(); 

	ImGui::Separator(); 

	if (selectedAction != nullptr)
		selectedAction->DrawUISettings();

	if(setScroll)
		ImGui::SetScrollHere(0.999f);
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
	flog("Object Data Reset"); 
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
	ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0.12f, 0.14f, 0.17f, 1.00f));
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(5, 5));

	ImGui::BeginChild("##AttributesChild", ImVec2(ImGui::GetContentRegionAvailWidth(), 200));

	int pos = 0;
	for (auto& currentAction : creatingObject->GetActionsList())
	{
		ActionSelectableInfo selectableInfo = currentAction->GetActionSelectableInfo();

		if (currentAction->GetType() == ACTION_DISPLAY_IMAGE)
		{
			DisplayImageAction* displayImageAction = (DisplayImageAction*)currentAction;

			if (displayImageAction->fromAnimation == true)
				continue; 
		}

		DrawSelectable(selectableInfo, currentAction->IsSelected(), pos, 42, currentAction);
		pos++;
	}

	ImGui::EndChild();

	ImGui::PopStyleVar();
	ImGui::PopStyleColor();
}

void ObjectCreatorDockPanel::DrawSequentialActionsList()
{
	ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0.12f, 0.14f, 0.17f, 1.00f));
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(5, 5));

	ImGui::BeginChild("##AttributesChildSequential", ImVec2(ImGui::GetContentRegionAvailWidth(), 200));

	int pos = 0;
	for (auto& currentAction : creatingObject->GetSequentialActionsList())
	{
		ActionSelectableInfo selectableInfo = currentAction->GetActionSelectableInfo();

		if (currentAction->GetType() == ACTION_DISPLAY_IMAGE)
		{
			DisplayImageAction* displayImageAction = (DisplayImageAction*)currentAction;

			if (displayImageAction->fromAnimation == true)
				continue;
		}

		DrawSelectable(selectableInfo, currentAction->IsSelected(), pos, 42, currentAction);
		pos++;
	}

	ImGui::EndChild();

	ImGui::PopStyleVar();
	ImGui::PopStyleColor();
}

bool ObjectCreatorDockPanel::DrawSelectable(ActionSelectableInfo selectableInfo, bool& isSelected, int posInList, int selectableHeight = 42, Action * currentAction = nullptr)
{
	bool ret = false; 
	ImGui::PushFont(App->moduleImGui->rudaBoldMid);

	Texture* imageIcon = App->moduleManager->GetIconFromActionType(selectableInfo.actionType);
	ImGui::SetCursorPos(ImVec2(10, 5 + (selectableHeight * posInList)));
	ImGui::Image((ImTextureID)imageIcon->GetTextureID(), ImVec2(30, 30), ImVec2(0, 1), ImVec2(1, 0));

	ImGui::SetCursorPos(ImVec2(50, (selectableHeight * posInList) + 4));
	if (ImGui::Selectable(selectableInfo.actionName.c_str(), &isSelected, ImGuiSelectableFlags_None, ImVec2(ImGui::GetContentRegionMax().x, selectableHeight - 3))) 
	{
		creatingObject->SetSelectedAction(selectableInfo.actionType, currentAction->IsActionSequential());
		selectedAction = currentAction;
		ret = true; 
	}
	ImGui::PopFont();

	// Description -----
	ImGui::SetCursorPosY((selectableHeight * posInList) + 24);
	ImGui::SetCursorPosX(ImGui::GetCursorPos().x + 52);

	ImGui::PushFont(App->moduleImGui->rudaRegularTiny);
	ImGui::TextWrapped(selectableInfo.actionDescription.c_str());
	ImGui::PopFont();

	return ret; 
}

void ObjectCreatorDockPanel::DrawSelectedActionSettings()
{

	if (selectedAction != nullptr)
	{
		ImGui::Spacing(); 
		selectedAction->DrawUISettings(); 
	}

	/*if (selectedAction)
	{
		ImGui::Spacing();

		switch (selectedAction->GetType())
		{
		case ACTION_DISPLAY_IMAGE:
			DrawDisplayImageSettings();
			break;

		case ACTION_CHANGE_ROOM:
			DrawChangeRoomActionSettings();
			break;

		case ACTION_MOD_VARIABLE:
			DrawModifyVariableActionSettings();
			break;

		case ACTION_EMIT_SOUND:
			DrawEmitSoundActionSettings();
			break;

		case ACTION_DISPLAY_ANIMATION:
			DrawDisplayAnimationSettings();
			break;

		case ACTION_FOLLOW_PATH:
			DrawFollowPathSettings();
			break;
		}
	}*/
}

void ObjectCreatorDockPanel::DrawFollowPathSettings()
{
	FollowPathAction* followPathAction = (FollowPathAction*)this->selectedAction;

	if (followPathAction != nullptr)
	{
		if (ImGui::CollapsingHeader("Follow Path Settings", ImGuiTreeNodeFlags_DefaultOpen))
			followPathAction->DrawUISettings();
	}
}

void ObjectCreatorDockPanel::DrawChangeRoomActionSettings()
{
	if (ImGui::CollapsingHeader("Change Room Attributes:", ImGuiTreeNodeFlags_DefaultOpen))
	{
		ChangeRoomAction* changeRoomAction = (ChangeRoomAction*)this->selectedAction;

		ImGui::PushFont(App->moduleImGui->rudaBoldBig);
		ImGui::Text("Action Happens On:");
		ImGui::PopFont();

		ImGui::PushFont(App->moduleImGui->rudaRegularMid);
		ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0.12f, 0.14f, 0.17f, 1.00f));

		ImGui::BeginChild("##OccChild", ImVec2(ImGui::GetContentRegionAvailWidth(), 70));

		ImGui::SetCursorPos(ImVec2(5, 8));
		ImGui::Checkbox("Object Clicked", &changeRoomAction->IsOccObjectClicked());

		ImGui::SetCursorPos(ImVec2(5, 38));
		ImGui::Checkbox("Blackboard Value Condition", &changeRoomAction->IsOccCondition());

		ImGui::SameLine();
		if (ImGui::Button(showValueConditionButtonText.c_str()))
		{
			if (showValueConditions)
			{
				showValueConditions = false;
				showValueConditionButtonText = "Show Conditions";
			}
			else
			{
				showValueConditions = true;
				showValueConditionButtonText = "Hide Conditions";
			}
		}

		ImGui::Spacing();
		ImGui::EndChild();

		if (showValueConditions)
			changeRoomAction->DrawActionConditionsList();

		ImGui::PopFont();
		ImGui::PopStyleColor();

		IMGUI_SPACED_SEPARATOR;

		ImGui::PushFont(App->moduleImGui->rudaBoldBig);
		ImGui::Text("Change Room Settings: ");
		ImGui::PopFont();

		changeRoomAction->DrawSelectDestinationCombo();
	}
}

void ObjectCreatorDockPanel::DrawModifyVariableActionSettings()
{
	if (ImGui::CollapsingHeader("Modify Variable Attributes:", ImGuiTreeNodeFlags_DefaultOpen))
	{
		ModifyVariableAction* modifyVariableAction = (ModifyVariableAction*)this->selectedAction;

		if (modifyVariableAction == nullptr)
			return;

		// Object Occurrence ---------
		ImGui::PushFont(App->moduleImGui->rudaBoldBig);
		ImGui::Text("Action Happens On:");
		ImGui::PopFont();

		ImGui::PushFont(App->moduleImGui->rudaRegularMid);
		ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0.12f, 0.14f, 0.17f, 1.00f));

		if (modifyVariableAction->GetActionClass() == ACTION_CLASS_SEQUENTIAL)
		{
			ImGui::BeginChild("##OccChild", ImVec2(ImGui::GetContentRegionAvailWidth(), 70));
			ImGui::SetCursorPos(ImVec2(5, 8));
			ImGui::Checkbox("Object Clicked", &modifyVariableAction->IsOccObjectClicked());
			ImGui::SetCursorPos(ImVec2(5, 38));
			ImGui::Checkbox("Blackboard Value Condition", &modifyVariableAction->IsOccCondition());

		}
		else if(modifyVariableAction->GetActionClass() == ACTION_CLASS_DIRECT)
		{
			ImGui::BeginChild("##OccChild", ImVec2(ImGui::GetContentRegionAvailWidth(), 132));
			ImGui::SetCursorPos(ImVec2(5, 8));
			ImGui::Checkbox("Enter Room", &modifyVariableAction->IsOccSceneEnter());
			ImGui::SetCursorPos(ImVec2(5, 38));
			ImGui::Checkbox("Exit Room", &modifyVariableAction->IsOccSceneLeave());
			ImGui::SetCursorPos(ImVec2(5, 68));
			ImGui::Checkbox("Object Clicked", &modifyVariableAction->IsOccObjectClicked());
			ImGui::SetCursorPos(ImVec2(5, 98));
			ImGui::Checkbox("Blackboard Value Condition", &modifyVariableAction->IsOccCondition());
		}

		ImGui::SameLine();
		if (ImGui::Button(showValueConditionButtonText.c_str()))
		{
			if (showValueConditions)
			{
				showValueConditions = false;
				showValueConditionButtonText = "Show Conditions";
			}
			else
			{
				showValueConditions = true;
				showValueConditionButtonText = "Hide Conditions";
			}
		}

		ImGui::EndChild();

		if (showValueConditions)
			modifyVariableAction->DrawActionConditionsList();

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

void ObjectCreatorDockPanel::DrawEmitSoundActionSettings()
{
	if (ImGui::CollapsingHeader("Emit Sound Attributes:", ImGuiTreeNodeFlags_DefaultOpen))
	{
		EmitSoundAction* emitSoundAction = (EmitSoundAction*)this->selectedAction;

		ImGui::PushFont(App->moduleImGui->rudaBoldBig);
		ImGui::Text("Action Happens On:");
		ImGui::PopFont();

		ImGui::PushFont(App->moduleImGui->rudaRegularMid);
		ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0.12f, 0.14f, 0.17f, 1.00f));

		if (emitSoundAction->GetActionClass() == ACTION_CLASS_SEQUENTIAL)
		{
			ImGui::BeginChild("##OccChild", ImVec2(ImGui::GetContentRegionAvailWidth(), 70));

			ImGui::SetCursorPos(ImVec2(5, 8));
			ImGui::Checkbox("Object Clicked", &emitSoundAction->IsOccObjectClicked());

			ImGui::SetCursorPos(ImVec2(5, 38));
			ImGui::Checkbox("Blackboard Value Condition", &emitSoundAction->IsOccCondition());
		}
		else if (emitSoundAction->GetActionClass() == ACTION_CLASS_DIRECT)
		{
			ImGui::BeginChild("##OccChild", ImVec2(ImGui::GetContentRegionAvailWidth(), 132));

			ImGui::SetCursorPos(ImVec2(5, 8));
			ImGui::Checkbox("Scene Enter", &emitSoundAction->IsOccSceneEnter());

			ImGui::SetCursorPos(ImVec2(5, 38));
			ImGui::Checkbox("Scene Leave", &emitSoundAction->IsOccSceneLeave());

			ImGui::SetCursorPos(ImVec2(5, 68));
			ImGui::Checkbox("Object Clicked", &emitSoundAction->IsOccObjectClicked());

			ImGui::SetCursorPos(ImVec2(5, 98));
			ImGui::Checkbox("Blackboard Value Condition", &emitSoundAction->IsOccCondition());
		}

		ImGui::SameLine();
		if (ImGui::Button(showValueConditionButtonText.c_str()))
		{
			if (showValueConditions)
			{
				showValueConditions = false;
				showValueConditionButtonText = "Show Conditions";
			}
			else
			{
				showValueConditions = true;
				showValueConditionButtonText = "Hide Conditions";
			}
		}

		ImGui::Spacing();
		ImGui::EndChild();

		if (showValueConditions)
			emitSoundAction->DrawActionConditionsList();

		ImGui::PopFont();
		ImGui::PopStyleColor();

		IMGUI_SPACED_SEPARATOR;

		ImGui::PushFont(App->moduleImGui->rudaBoldBig);
		ImGui::Text("Emit Sound Settings: ");
		ImGui::PopFont();

		static char soundNameBuffer[256] = "";

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

					strcpy(soundNameBuffer, resourceDropped->GetName().c_str());
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
				strcpy(soundNameBuffer, selectedSound->GetName().c_str());
			}
		}
	}
}

void ObjectCreatorDockPanel::DrawDisplayAnimationSettings()
{
	DisplayAnimationAction* displayAnimation = (DisplayAnimationAction*)this->selectedAction;
	
	if (displayAnimation != nullptr)
	{
		if(ImGui::CollapsingHeader("Animation Settings", ImGuiTreeNodeFlags_DefaultOpen))
			displayAnimation->DrawUISettings(); 
	}
}

void ObjectCreatorDockPanel::OnAddActionButtonClicked(bool fromFixedAction)
{
	if (ImGui::BeginPopup("AddActionToObject"))
	{	
		ImGui::Spacing();

		// Search Bar ---------------
		ImGui::InputTextWithHint("##SearchTool", "Search...", searchNewActionBuffer, IM_ARRAYSIZE(searchNewActionBuffer));
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
			case ACTION_DISPLAY_IMAGE:
				selectedAction = creatingObject->AddDisplayImageAction(std::string(MyFileSystem::getInstance()->GetIconsDirectory() + "EmptyObject.png").c_str(), fromFixedAction);
				break;

			case ACTION_CHANGE_ROOM:
				creatingObject->AddChangeRoomAction();
				break;

			case ACTION_MOD_VARIABLE:
				creatingObject->AddModifyVariableAction();
				break;

			case ACTION_EMIT_SOUND:
				creatingObject->AddEmitSoundAction();
				break;

			case ACTION_DISPLAY_TEXT:
				creatingObject->AddDisplayTextAction();
				break;

			case ACTION_DISPLAY_ANIMATION:
				creatingObject->AddDisplayAnimationAction(fromFixedAction);
				break;

			case ACTION_FOLLOW_PATH:
				creatingObject->AddFollowPathAction();
				break;

			case AT_null:
				break;
			}

			ImGui::EndChild();
			ImGui::PopStyleColor();
			ImGui::PopStyleVar();	
			ImGui::CloseCurrentPopup();
			ImGui::EndPopup();
			return; 
		}

		ImGui::EndChild();
		ImGui::PopStyleColor();
		ImGui::PopStyleVar();

		ImGui::EndPopup();
	}
}

void ObjectCreatorDockPanel::Close()
{
	creatingObject->CleanUp(); 
	delete creatingObject; 

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

			ImGui::PushFont(App->moduleImGui->rudaRegularTiny);
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
		
		ImGui::SetCursorPos(ImVec2((ImGui::GetContentRegionAvailWidth() / 2) - 76, (previewTextureMaxSize / 2) - (75)));
		Texture* emptyObject = (Texture*)ResourceManager::getInstance()->GetResource("EmptyObject"); 
		ImGui::Image((ImTextureID)emptyObject->GetTextureID(), ImVec2(150, 150));

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
	DisplayImageAction* displayImageAction = (DisplayImageAction*)creatingObject->GetAction(ACTION_DISPLAY_IMAGE);
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

bool ObjectCreatorDockPanel::DrawCloseAndCreateButton()
{
	ImGui::PushFont(App->moduleImGui->rudaBlackMid);
	ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(1,0,0, 0.1f)); 
	ImGui::SetCursorPosX(ImGui::GetContentRegionMax().x - 205);

	if (ImGui::Button("Close", ImVec2(100, 30)))
	{
		ToggleVisibility();
		Close();

		ImGui::PopStyleColor();
		ImGui::PopFont();
		return true; 
	}

	ImGui::SameLine(); 
	ImGui::PopStyleColor();
	ImGui::PopFont();

	ImGui::SetCursorPosX(ImGui::GetContentRegionMax().x - 100);
	ImGui::PushFont(App->moduleImGui->rudaBlackMid);
	if (ImGui::Button("Create", ImVec2(100, 30)))
	{
		std::string newObjectNameStr(newObjectName);
		if (newObjectNameStr.empty()) 
		{
			FLY_ERROR("Object with no name can not be created");
			ImGui::PopFont();
			return false;
		}
		else
		{
			ToggleVisibility();
			AddCreatingObject();
			
			ImGui::PopFont();
			return false;
		}	
	}

	ImGui::PopFont();

	return false; 
}

void ObjectCreatorDockPanel::AddCreatingObject()
{
	// If is item inventory and has no image assigned, we create a display image component with a default empty image 
	if (creatingObject->IsInventoryItem() && creatingObject->GetAction(ACTION_DISPLAY_IMAGE) == nullptr)	
		creatingObject->AddDisplayImageAction(); 
	
	// Clickable Area
	if (clickableAreaActive)
	{
		if (!creatingObject->HasVisuals())
		{
			creatingObject->CreateClickableArea(clickableAreaPosPerc, clickableAreaSizePerc, true);
			creatingObject->drawClickableArea = true;
		}
		else
		{
			creatingObject->CreateClickableArea(clickableAreaPosPerc, clickableAreaSizePerc);
			creatingObject->drawClickableArea = true;
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

	creatingObject->SetParentRoom(App->moduleRoomManager->GetSelectedRoom()); 
}

void ObjectCreatorDockPanel::DrawDisplayImageSettings()
{
	if (ImGui::CollapsingHeader("Display Image Attributes:", ImGuiTreeNodeFlags_DefaultOpen))
	{
		DisplayImageAction* selectedImageAction = (DisplayImageAction*)this->selectedAction;

		static char buf[256] = "";
		ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0.12f, 0.14f, 0.17f, 1.00f));
		ImGui::BeginChild("UI_ImagePreview", ImVec2(ImGui::GetContentRegionAvailWidth(), 195));

		ImVec2 imageMaxSize = ImVec2(ImGui::GetContentRegionAvailWidth(), 135);
		ImVec2 uiImageDimensions = ImVec2(150, 150);
		Texture* imageTexture = selectedImageAction->GetTexture();
		ImTextureID selectedTextureID = 0;

		if (imageTexture != nullptr)
		{
			float aspectRatio = imageTexture->GetAspectRatio();

			if (imageTexture->IsVertical())
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
			selectedTextureID = (ImTextureID)imageTexture->GetTextureID();
		}

		ImGui::Spacing();

		ImGui::SetCursorPos(ImVec2(ImGui::GetContentRegionAvailWidth() / 2 - (uiImageDimensions.x / 2), imageMaxSize.y / 2 - (uiImageDimensions.y / 2) + 10));
		ImGui::Image(selectedTextureID, uiImageDimensions);

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
					selectedImageAction->SetTexture(textureDropped);
				}
			}

			ImGui::EndDragDropTarget();
		}

		ImGui::SameLine();
		if (ImGui::Button("Search##SearchUIIMage"))
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
				flog("Player Opened %s", path);
			}
		}
		ImGui::EndChild();
		ImGui::PopStyleColor();
	
		//{
		//	//ImGui::PushFont(App->moduleImGui->rudaBoldHuge);
		//			//ImGui::Text("Display Image Attributes:");
		//			//ImGui::PopFont();

		//			//ImGui::Separator();

		//	static char buf[256] = "";
		//	DisplayImageAction* selectedImageAction = (DisplayImageAction*)this->selectedAction;

		//	// Object Occurrence -----------------------
		//	selectedImageAction->DrawActionOccurenceCheckboxes();

		//	IMGUI_SPACE_SEPARATOR;

		//	// Settings ---------------------------------
		//	//if (selectedImageAction->GetTexture() == nullptr)
		//	//	selectedImageAction->SetTexture((Texture*)ResourceManager::getInstance()->GetResource("EmptyObject"));
		//	//else
		//	//	selectedImageAction->SetTexture((Texture*)ResourceManager::getInstance()->GetResource(selectedImageAction->GetTexture()->GetName()));

		//	float aspect_ratio = selectedImageAction->GetTexture()->GetAspectRatio();
		//	float previewQuadWidth = 150;
		//	float previewQuadHeight = previewQuadWidth / aspect_ratio;

		//	PUSH_FONT(App->moduleImGui->rudaRegularMid);
		//	ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0.12f, 0.14f, 0.17f, 1.00f));

		//	int childHeight = previewQuadHeight + 20;

		//	PUSH_FONT(App->moduleImGui->rudaBoldBig);
		//	ImGui::Text("Image Settings:");
		//	POP_FONT;

		//	ImGui::BeginChild("##4ShowImage", ImVec2(ImGui::GetContentRegionAvailWidth(), childHeight));

		//	ImGui::Columns(2);
		//	ImGui::SetColumnWidth(0, previewQuadWidth + 10);

		//	ImGui::Spacing();
		//	ImGui::Image((ImTextureID)selectedImageAction->GetTexture()->GetTextureID(), ImVec2(previewQuadWidth, previewQuadHeight));

		//	ImGui::NextColumn();

		//	ImGui::Spacing();
		//	ImGui::Text("Name: "); ImGui::SameLine();
		//	ImGui::TextColored(ImVec4(0.1f, 0.7f, 1.0f, 1.0f), "%s", selectedImageAction->GetTexture()->GetName().c_str());

		//	ImGui::Text("Width: "); ImGui::SameLine();
		//	ImGui::TextColored(ImVec4(0.1f, 0.7f, 1.0f, 1.0f), "%d", selectedImageAction->GetTexture()->GetWidth());

		//	ImGui::Text("Height: "); ImGui::SameLine();
		//	ImGui::TextColored(ImVec4(0.1f, 0.7f, 1.0f, 1.0f), "%d", selectedImageAction->GetTexture()->GetHeigth());

		//	Texture* searchTexture = (Texture*)ResourceManager::getInstance()->GetResource("SearchIcon");
		//	if (ImGui::Button("Change Image"))
		//	{
		//		char const* lFilterPatterns[2] = { "*.jpg" , "*.png" };
		//		const char* path = tinyfd_openFileDialog("Load Image...", NULL, 2, lFilterPatterns, NULL, 0);

		//		if (path != NULL)
		//		{
		//			if (!ResourceManager::getInstance()->ExistResourcePath(path))
		//			{
		//				selectedImageAction->SetTexture(ImageImporter::getInstance()->LoadTexture(path, false));
		//				ResourceManager::getInstance()->AddResource(selectedImageAction->GetTexture(), selectedImageAction->GetTexture()->GetName());
		//			}
		//			else
		//			{
		//				selectedImageAction->SetTexture((Texture*)ResourceManager::getInstance()->GetResourceByPath(path));
		//			}

		//			strcpy(buf, path);
		//			flog("Player Opened %s", path);
		//		}

		//	}
		//	ImGui::PopStyleColor();
		//	ImGui::EndChild();

		//	POP_FONT;
		//}
	
	}	
}

void ObjectCreatorDockPanel::DrawAddAndDeleteActionButtons(bool fromFixedAction)
{
	ImGui::Spacing();
	ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(2, 2));

	ImGui::SetCursorPosY(ImGui::GetCursorPosY() - 5);
	Texture* plusIconTex = (Texture*)ResourceManager::getInstance()->GetResource("PlusIconWhite");

	if (ImGui::ImageButton((ImTextureID)plusIconTex->GetTextureID(), ImVec2(30, 30)))
	{
		ImGui::OpenPopup("AddActionToObject");
	}

	ImGui::SameLine();
	Texture* minusIconTex = (Texture*)ResourceManager::getInstance()->GetResource("MinusIconWhite");
	if (ImGui::ImageButton((ImTextureID)minusIconTex->GetTextureID(), ImVec2(30, 30)))
	{
		if (selectedAction != nullptr) 
		{
			creatingObject->DeleteAction(selectedAction->GetType());
			selectedAction = nullptr; 
		}
	}

	ImGui::PopStyleVar();

	// Callbacks for buttons 
	OnAddActionButtonClicked(fromFixedAction);
}
