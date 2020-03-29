#include "DialogueStep.h"
#include "DialogueText.h"
#include "DisplayTextAction.h"
#include "StepAnswer.h"

DialogueStep::DialogueStep(string _dialogueText)
{
	dialogueText = new DialogueText();
	dialogueText->SetDialogueText(_dialogueText);
}

DialogueStep::~DialogueStep()
{
	
}

void DialogueStep::SetDialogueText(string _dialogueText)
{
	dialogueText->SetDialogueText(_dialogueText);
}

void DialogueStep::AddStepAnswer(string _answerText)
{
	StepAnswer* newStepAnswer = new StepAnswer(); 
	answersList.push_back(newStepAnswer); 
}
