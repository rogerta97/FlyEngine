#include "DialogueSlot.h"
#include "DialogueText.h"
#include "DisplayTextAction.h"
#include "StepAnswer.h"
#include "NodeGraph.h"

#include "Application.h"
#include "ModuleManager.h"
#include "DialogueEditorDockPanel.h"

DialogueSlot::DialogueSlot(string _dialogueText)
{
	dialogueText = new DialogueText();
	dialogueText->SetDialogueText(_dialogueText);
}

DialogueSlot::~DialogueSlot()
{
	
}

void DialogueSlot::SetDialogueText(string _dialogueText)
{
	dialogueText->SetDialogueText(_dialogueText);
}

void DialogueSlot::AddStepAnswer(string _answerText)
{
	// Add Answer ----
	StepAnswer* newStepAnswer = new StepAnswer(); 
	answersList.push_back(newStepAnswer); 

	// Add Node Slot 
	NodeGraph* dialoguesNodeGraph = App->moduleManager->GetCurrentDialogueEditor()->GetNodeGraph();

	if (dialoguesNodeGraph != nullptr)
		dialoguesNodeGraph->CreateNode(_stepText, ImVec2(0, 0));

	return newDialogueSlot;
}
