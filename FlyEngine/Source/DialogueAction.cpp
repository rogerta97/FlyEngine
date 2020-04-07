#include "DialogueAction.h"
#include "imgui.h"
#include "Dialogue.h"
#include "DialogueStep.h"

#include "Application.h"
#include "ModuleImGui.h"
#include "ResourceManager.h"
#include "Texture.h"

#include "mmgr.h"


DialogueAction::DialogueAction(FlyObject* _parentObject)
{
	actionType = ACTION_DIALOGUE;
	isVisual = false;
	showVariableConditions = false; 

	SetActionName("Dialog");
	SetToolDescription("This should be the description of the dialog");

	dialogue = new Dialogue(); 
}

DialogueAction::~DialogueAction()
{
}

void DialogueAction::SaveAction(JSON_Object* jsonObject, string serializeObjectString, bool literalStr, int actionPositionInObject)
{
	string actionSerializeSection;

	if (!literalStr)
		actionSerializeSection = serializeObjectString + string("Actions.Action_") + to_string(actionPositionInObject) + ".";
	else
		actionSerializeSection = serializeObjectString;

	Action::SaveAction(jsonObject, actionSerializeSection);
	Action::SaveOccurrence(jsonObject, actionSerializeSection);
}

void DialogueAction::DrawUISettings()
{
	if (ImGui::CollapsingHeader("Dialogue Settings:", ImGuiTreeNodeFlags_DefaultOpen))
	{
		DrawActionOccurenceCheckboxes();

		ImGui::Separator();

		ImGui::PushFont(App->moduleImGui->rudaBoldBig);
		ImGui::Text("Dialogue Steps");
		ImGui::PopFont();

		PUSH_CHILD_BG_COLOR_DARK;
		ImGui::BeginChild("Steps Dialogue Holder", ImVec2(ImGui::GetContentRegionAvail().x, 150)); 

		for (auto& currentStep : dialogue->GetDialogueSteps())
		{
			ImGui::Selectable(currentStep->GetStepTextStr().c_str());
		}

		ImGui::EndChild();
		ImGui::PopStyleColor(); 

		Texture* plusTexture = (Texture*)ResourceManager::getInstance()->GetResource("PlusIconWhite2"); 
		if (ImGui::ImageButton((ImTextureID)plusTexture->GetTextureID(), ImVec2(35, 35)))
		{
			DialogueStep* newStep = AddDialogueStep("This Is a Sentence"); 
			newStep->AddStepAnswer("I'm bored, this place is a shit");
			newStep->AddStepAnswer("Happy, just give me more of that she");
		}

		ImGui::Separator();
	}
}

void DialogueAction::DrawActionOccurenceCheckboxes()
{
	ImGui::PushFont(App->moduleImGui->rudaBoldBig);
	ImGui::Text("Action Happens On:");
	ImGui::PopFont();

	ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0.12f, 0.14f, 0.17f, 1.00f));

	if (GetActionClass() == ACTION_CLASS_SEQUENTIAL)
	{
		ImGui::BeginChild("##OccChild", ImVec2(ImGui::GetContentRegionAvailWidth(), 35));
		ImGui::SetCursorPos(ImVec2(5, 5));
		ImGui::Checkbox("Object Condition", &occ_blackboardValue);
	}
	else if (GetActionClass() == ACTION_CLASS_DIRECT)
	{
		ImGui::BeginChild("##OccChild", ImVec2(ImGui::GetContentRegionAvailWidth(), 100));
		ImGui::SetCursorPos(ImVec2(5, 5));
		ImGui::Checkbox("Scene Enter", &occ_SceneEnter);
		ImGui::SetCursorPos(ImVec2(5, 35));
		ImGui::Checkbox("Object Clicked", &occ_ObjectClicked);
		ImGui::SetCursorPos(ImVec2(5, 65));
		ImGui::Checkbox("Object Condition", &occ_blackboardValue);
	}

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
	ImGui::Spacing();

	if (showVariableConditions)
		DrawActionConditionsList();

	ImGui::PopStyleColor();

}

DialogueStep* DialogueAction::AddDialogueStep(string _dialogueSlotText)
{
	return dialogue->AddDialogueStep(_dialogueSlotText);
}

Dialogue* DialogueAction::GetDialogueData()
{
	return dialogue;
}
