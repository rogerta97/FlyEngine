#include "DialogueSlot.h"
#include "DialogueText.h"
#include "DisplayTextAction.h"
#include "StepAnswer.h"

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
	StepAnswer* newStepAnswer = new StepAnswer(); 
	answersList.push_back(newStepAnswer); 
}
