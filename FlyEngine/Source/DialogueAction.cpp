#include "DialogueAction.h"
#include "imgui.h"
#include "Dialogue.h"
#include "DialogueStep.h"
#include "mmgr.h"

DialogueAction::DialogueAction(FlyObject* _parentObject)
{
	actionType = ACTION_DIALOGUE;
	isVisual = false;

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
		if (ImGui::Button("Open Dialogue Editor"))
		{

		}

		if (ImGui::Button("Add Empty Slot"))
		{
			DialogueStep* newStep = AddDialogueStep(); 
		}
	}
}

DialogueStep* DialogueAction::AddDialogueStep()
{
	return dialogue->AddDialogueStep(); 
}