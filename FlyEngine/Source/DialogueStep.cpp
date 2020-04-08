#include "DialogueStep.h"
#include "DialogueText.h"
#include "DisplayTextAction.h"
#include "StepAnswer.h"
#include "NodeGraph.h"

#include "Application.h"
#include "ModuleManager.h"
#include "DialogueEditorDockPanel.h"

DialogueStep::DialogueStep(string _dialogueText)
{
	dialogueText = new DialogueText();
	dialogueText->SetDialogueText(_dialogueText);
	stepUID = RandomNumberGenerator::getInstance()->GenerateUID(); 
	SetName("StepName...");
}

DialogueStep::~DialogueStep()
{
	
}

void DialogueStep::SetStepText(string _dialogueText)
{
	dialogueText->SetDialogueText(_dialogueText);
}

string DialogueStep::GetStepTextStr()
{
	if (dialogueText != nullptr)
	{
		return dialogueText->GetTextAction()->GetText();
	}

	return string();
}

void DialogueStep::AddStepAnswer(string _answerText)
{
	// Add Answer ----
	StepAnswer* newStepAnswer = new StepAnswer(); 
	newStepAnswer->SetAnswerText(_answerText);
	answersList.push_back(newStepAnswer); 
}

list<StepAnswer*>& DialogueStep::GetAnswersList()
{
	return answersList;
}

string DialogueStep::GetName()
{
	return stepName;
}

void DialogueStep::SetName(string newName)
{
	stepName = newName; 
}

UID DialogueStep::GetUID() const
{
	return stepUID;
}
