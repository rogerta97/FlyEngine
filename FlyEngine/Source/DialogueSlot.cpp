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
	slotUID = RandomNumberGenerator::getInstance()->GenerateUID(); 
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
	newStepAnswer->SetAnswerText(_answerText);
	answersList.push_back(newStepAnswer); 
}

UID DialogueSlot::GetUID() const
{
	return slotUID;
}
