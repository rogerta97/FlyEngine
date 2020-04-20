#include "CharacterCreatorDockPanel.h"
#include "imgui.h"
#include "Texture.h"
#include "ResourceManager.h"
#include "FlyObjectCharacter.h"
#include "DisplayImageAction.h"
#include "FlyObject.h"

#include "Application.h"
#include "ModuleImGui.h"
#include "ModuleManager.h"

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

		ImGui::SetCursorPosY(ImGui::GetCursorPosY() - 5);
		IMGUI_SPACED_SEPARATOR;

		if (ImGui::CollapsingHeader("Character Animations", ImGuiTreeNodeFlags_DefaultOpen))
		{

		}

		ImGui::PushFont(App->moduleImGui->rudaBlackBig);
		ImGui::Text("Sequence Actions:");
		ImGui::PopFont();

		PUSH_CHILD_BG_COLOR_DARK;
		INC_CURSOR_4;
		ImGui::BeginChild("Holder Character Actions", ImVec2(ImGui::GetContentRegionAvail().x - 4, 200));

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
			if (ImGui::Selectable(selectableInfo.actionName.c_str(), false, ImGuiSelectableFlags_None, ImVec2(ImGui::GetContentRegionMax().x, 42 - 3)))
			{
				creatingCharacterFO->SetSelectedAction(selectableInfo.actionType, currentAction->IsActionSequential());
				//selectedAction = currentAction;
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

		ImGui::EndChild();
		ImGui::PopStyleColor();

		if (ImGui::Button("Add Action"))
		{

		}

		ImGui::SameLine(); 
		if (ImGui::Button("Delete Action"))
		{

		}


	}
	ImGui::End();

	return true;
}
