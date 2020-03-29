#include "StepAnswer.h"
#include "DialogueText.h"

StepAnswer::StepAnswer(string newAnswerText)
{
	answerDialogueText = new DialogueText();
	answerDialogueText->SetDialogueText(newAnswerText);  

	destinationStep = nullptr; 
}

StepAnswer::~StepAnswer()
{

}

void StepAnswer::SetAnswerText(string newAnswerText)
{
	answerDialogueText->SetDialogueText(newAnswerText); 
}

void StepAnswer::SetDestinationStep(DialogueSlot* dstStep)
{
	destinationStep = dstStep;
}

void StepAnswer::AddCallbackAction(Action* newCallbackAction)
{
	if(newCallbackAction != nullptr)
		callbackActions.push_back(newCallbackAction);
}

DialogueText* StepAnswer::GetAnswerDialogueText()
{
	return answerDialogueText;
}

DialogueSlot* StepAnswer::GetDestinationStep()
{
	return destinationStep;
}
