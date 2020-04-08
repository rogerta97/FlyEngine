#include "StepAnswer.h"
#include "DialogueText.h"
#include "RandomNumberGenerator.h"

StepAnswer::StepAnswer(string newAnswerText)
{
	answerDialogueText = new DialogueText();
	answerDialogueText->SetDialogueText(newAnswerText);  
	answerUID = RandomNumberGenerator::getInstance()->GenerateUID(); 
	SetName("Answer..."); 

	destinationStep = nullptr; 
}

StepAnswer::~StepAnswer()
{

}

void StepAnswer::SetAnswerText(string newAnswerText)
{
	answerDialogueText->SetDialogueText(newAnswerText); 
}

void StepAnswer::SetDestinationStep(DialogueStep* dstStep)
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

DialogueStep* StepAnswer::GetDestinationStep()
{
	return destinationStep;
}

string StepAnswer::GetName()
{
	return answerName;
}

void StepAnswer::SetName(string newName)
{
	answerName = newName; 
}

UID StepAnswer::GetUID()
{
	return answerUID;
}
