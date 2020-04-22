#include "CharacterCreatorDockPanel.h"
#include "imgui.h"
#include "Texture.h"
#include "ResourceManager.h"
#include "FlyObjectCharacter.h"
#include "DisplayAnimationAction.h"
#include "MyFileSystem.h"
#include "DisplayImageAction.h"
#include "Room.h"
#include "GameViewportDockPanel.h"
#include "FlyObject.h"

#include "Application.h"
#include "ModuleImGui.h"
#include "ModuleManager.h"
#include "ModuleRoomManager.h"

#include "mmgr.h"

CharacterCreatorDockPanel::CharacterCreatorDockPanel(bool isVisible) : DockPanel("Character Creator", isVisible)
{
	flyEngineSection = FLY_SECTION_null;
	dockPanelType = DOCK_DIALOGUE_EDITOR;
}

CharacterCreatorDockPanel::~CharacterCreatorDockPanel()
{
}

void CharacterCreatorDockPanel::ResetObjectData()
{
	flog("Object Data Reset");
	//selectedAction = nullptr;
	creatingCharacterFO = new FlyObjectCharacter("Prev");
	creatingCharacterFO->InitCharacter();
}

void CharacterCreatorDockPanel::ToggleVisiblity()
{
	SetVisible(!IsVisible());
}

bool CharacterCreatorDockPanel::Draw()
{
	if (!DockPanel::Draw())
		return false;

	if (ImGui::Begin(panelName.c_str(), &visible, ImGuiWindowFlags_NoTitleBar))
	{
		Texture* iconTexture = (Texture*)ResourceManager::getInstance()->GetResource("AddCharacterIcon");

		ImGui::Image((ImTextureID)iconTexture->GetTextureID(), ImVec2(35, 35));
		ImGui::SameLine();

		ImGui::SetCursorPos(ImVec2(ImGui::GetCursorPosX() + 7, ImGui::GetCursorPosY() + 10));
		ImGui::PushFont(App->moduleImGui->rudaBlackHuge);
		ImGui::Text("Character Creator:");
		ImGui::PopFont();

		ImGui::SameLine();
		ImGui::SetCursorPosX(ImGui::GetContentRegionMax().x - 100);
		ImGui::PushFont(App->moduleImGui->rudaBlackMid);

		if (ImGui::Button("Create##CreateCharacter", ImVec2(100, 30)))
		{
			std::string newObjectNameStr(newObjectName);
			if (newObjectNameStr.empty())
			{
				FLY_ERROR("Object with no name can not be created");
				ImGui::PopFont();
				ImGui::End();
				return false;
			}
			else
			{
				ToggleVisibility();
				//AddCharacterObject();
				//if (creatingCharacterFO->IsInventoryItem() && creatingObject->GetAction(ACTION_DISPLAY_IMAGE) == nullptr)
				//	creatingObject->AddDisplayImageAction();

				//// Clickable Area
				//if (clickableAreaActive)
				//{
				//	if (!creatingObject->HasVisuals())
				//	{
				//		creatingObject->CreateClickableArea(clickableAreaPosPerc, clickableAreaSizePerc, true);
				//		creatingObject->drawClickableArea = true;
				//	}
				//	else
				//	{
				//		creatingObject->CreateClickableArea(clickableAreaPosPerc, clickableAreaSizePerc);
				//		creatingObject->drawClickableArea = true;
				//	}
				//	creatingObject->clickableAreaActive = true;
				//}

				//creatingObject->SetClickableAreaPosOne(clickableAreaPosPerc);
				//creatingObject->SetClickableAreaSizeOne(clickableAreaSizePerc);

				// Add Object
				creatingCharacterFO->SetName(newObjectName);
				App->moduleRoomManager->GetSelectedRoom()->AddFlyObject(creatingCharacterFO);

				App->moduleRoomManager->GetSelectedRoom()->SetSelectedObject(creatingCharacterFO);
				App->moduleImGui->gameViewportDockPanel->SetGizmoMode(GizmoMode::GIZMO_SELECT);
				creatingCharacterFO->FitObjectUtils();

				creatingCharacterFO->SetParentRoom(App->moduleRoomManager->GetSelectedRoom());

				ImGui::PopFont();
				ImGui::End();
				return false;
			}
		}

		ImGui::PopFont();

		ImGui::SetCursorPosY(ImGui::GetCursorPosY() - 5);
		IMGUI_SPACED_SEPARATOR;

		ImGui::PushFont(App->moduleImGui->rudaRegularMid);
		ImGui::InputTextWithHint("Name##ObjectNaming", "Name...", newObjectName, 256 * sizeof(char));
		ImGui::PopFont();

		ImGui::InputTextMultiline("Description##ObjectDescription", newObjectDescription, 256 * sizeof(char), ImVec2(ImGui::GetContentRegionMax().x - 100, 100));
		
		ImGui::Spacing();
		ImGui::Separator();

		DrawCharacterAnimationsUI();
		
		ImGui::PushFont(App->moduleImGui->rudaBlackBig);
		ImGui::Text("Sequence Actions:");
		ImGui::PopFont();

		PUSH_CHILD_BG_COLOR_DARK;
		INC_CURSOR_4;
		ImGui::BeginChild("Holder Character Actions", ImVec2(ImGui::GetContentRegionAvail().x - 4, 200));

		DrawCharacterSequentialActionsUI();


		ImGui::EndChild();
		ImGui::PopStyleColor();

		if (ImGui::Button("Add Action"))
		{
			ImGui::OpenPopup("AddActionToObject");
		}

		ImGui::SameLine(); 
		if (ImGui::Button("Delete Action"))
		{

		}

		if (selectedActionUI != nullptr)
			selectedActionUI->DrawUISettings(); 

		if (ImGui::BeginPopup("AddActionToObject"))
		{
			ImGui::Spacing();

			// Search Bar ---------------
			static char searchNewActionBuffer[256];
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

			//if (toSequentialList)
			newActionSelected = App->moduleManager->DrawActionDictionaryUI();
			//else
			//	newActionSelected = App->moduleManager->DrawActionDictionaryUI(FILTER_ACTIONS_FIXED);

			if (newActionSelected != nullptr)
			{
				Action* selectedAction = nullptr;
				switch (newActionSelected->actionType)
				{
				case ACTION_DISPLAY_IMAGE:
					selectedAction = (Action*)creatingCharacterFO->AddDisplayImageAction(std::string(MyFileSystem::getInstance()->GetIconsDirectory() + "EmptyObject.png").c_str(), true);
					break;

				case ACTION_CHANGE_ROOM:
					selectedAction = (Action*)creatingCharacterFO->AddChangeRoomAction(true);
					break;

				case ACTION_MOD_VARIABLE:
					selectedAction = (Action*)creatingCharacterFO->AddModifyVariableAction(true);
					break;

				case ACTION_EMIT_SOUND:
					selectedAction = (Action*)creatingCharacterFO->AddEmitSoundAction(true);
					break;

				case ACTION_DISPLAY_TEXT:
					selectedAction = (Action*)creatingCharacterFO->AddDisplayTextAction(true);
					break;

				case ACTION_DISPLAY_ANIMATION:
					selectedAction = (Action*)creatingCharacterFO->AddDisplayAnimationAction(true);
					break;

				case ACTION_FOLLOW_PATH:
					selectedAction = (Action*)creatingCharacterFO->AddFollowPathAction(true);
					break;

				case ACTION_DIALOGUE:
					selectedAction = (Action*)creatingCharacterFO->AddDialogueAction(true);
					break;

				case AT_null:
					break;
				}

				if (selectedAction != nullptr)
					selectedAction->SetActionClass(ACTION_CLASS_SEQUENTIAL);

				ImGui::EndChild();
				ImGui::PopStyleColor();
				ImGui::PopStyleVar();
				ImGui::CloseCurrentPopup();
				ImGui::EndPopup();
				ImGui::End();
				return true;
			}

			ImGui::EndChild();
			ImGui::PopStyleColor();
			ImGui::PopStyleVar();
			ImGui::EndPopup();
		}
	}

	ImGui::End();

	return true;
}

void CharacterCreatorDockPanel::DrawCharacterSequentialActionsUI()
{
	int pos = 0;
	for (auto& currentAction : creatingCharacterFO->GetSequentialActionsList())
	{
		ActionSelectableInfo selectableInfo = currentAction->GetActionSelectableInfo();

		if (currentAction->GetType() == ACTION_DISPLAY_IMAGE)
		{
			DisplayImageAction* displayImageAction = (DisplayImageAction*)currentAction;

			if (displayImageAction->fromAnimation == true)
				continue;
		}

		bool ret = false;
		ImGui::PushFont(App->moduleImGui->rudaBoldMid);

		Texture* imageIcon = App->moduleManager->GetIconFromActionType(selectableInfo.actionType);
		ImGui::SetCursorPos(ImVec2(10, 5 + (42 * pos)));
		ImGui::Image((ImTextureID)imageIcon->GetTextureID(), ImVec2(30, 30), ImVec2(0, 1), ImVec2(1, 0));

		ImGui::SetCursorPos(ImVec2(50, (42 * pos) + 4));
		bool selected = false; 

		if (selectedActionUI != nullptr && selectedActionUI->GetUID() == currentAction->GetUID())
			selected = true; 

		if (ImGui::Selectable(selectableInfo.actionName.c_str(), selected, ImGuiSelectableFlags_None, ImVec2(ImGui::GetContentRegionMax().x, 42 - 3)))
		{
			creatingCharacterFO->SetSelectedAction(selectableInfo.actionType, currentAction->IsActionSequential());
			selectedActionUI = currentAction;
			ret = true;
		}
		ImGui::PopFont();

		// Description -----
		ImGui::SetCursorPosY((42 * pos) + 24);
		ImGui::SetCursorPosX(ImGui::GetCursorPos().x + 52);

		ImGui::PushFont(App->moduleImGui->rudaRegularTiny);
		ImGui::TextWrapped(selectableInfo.actionDescription.c_str());
		ImGui::PopFont();

		pos++;
	}
}

void CharacterCreatorDockPanel::DrawCharacterAnimationsUI()
{
	ImGui::PushFont(App->moduleImGui->rudaBlackBig);
	if (ImGui::TreeNode("Idle Animation"))
	{
		ImGui::PushFont(App->moduleImGui->rudaRegularMid);

		ImGui::Indent(-10);
		ImGui::BeginChild("Idle Anims Child", ImVec2(ImGui::GetContentRegionAvail().x, 335));
		creatingCharacterFO->GetIdleAnimation()->DrawUICharacterSettings("Animation Preview:");
		ImGui::EndChild();
		ImGui::PopFont(); 
		ImGui::Indent(10);
		ImGui::TreePop();
	}

	if (ImGui::TreeNode("Walk Animation"))
	{
		ImGui::PushFont(App->moduleImGui->rudaRegularMid);
		ImGui::Indent(-10);
		ImGui::BeginChild("Walk Anims Child", ImVec2(ImGui::GetContentRegionAvail().x, 335));
		creatingCharacterFO->GetWalkAnimation()->DrawUICharacterSettings("Animation Preview:");
		ImGui::EndChild();
		ImGui::Indent(10);
		ImGui::PopFont();
		ImGui::TreePop();
	}

	if (ImGui::TreeNode("Talk Animation"))
	{
		ImGui::PushFont(App->moduleImGui->rudaRegularMid);
		ImGui::Indent(-10);
		ImGui::BeginChild("Talk Anims Child", ImVec2(ImGui::GetContentRegionAvail().x, 335));
		creatingCharacterFO->GetTalkAnimation()->DrawUICharacterSettings("Animation Preview:");
		ImGui::EndChild();
		ImGui::Indent(10);
		ImGui::PopFont();
		ImGui::TreePop();
	}

	ImGui::PopFont(); 
	ImGui::Separator(); 
}
