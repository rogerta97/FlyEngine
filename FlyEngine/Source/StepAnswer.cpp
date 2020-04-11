#include "StepAnswer.h"
#include "DialogueText.h"
#include "DialogueStep.h"
#include "RandomNumberGenerator.h"
#include "DisplayTextAction.h"

StepAnswer::StepAnswer(DialogueStep* _parentStep, string newAnswerText, string newAnswerName)
{
	answerDialogueText = new DialogueText();
	answerDialogueText->SetDialogueText(newAnswerText);  
	answerUID = RandomNumberGenerator::getInstance()->GenerateUID(); 
	SetName(newAnswerName.c_str());
	parentStep = _parentStep;

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

void StepAnswer::SaveAnswer(JSON_Object* jsonObject, string serializeObjectString)
{
	json_object_dotset_string(jsonObject, string(serializeObjectString + "Name").c_str(), answerName.c_str());
	json_object_dotset_string(jsonObject, string(serializeObjectString + "Text").c_str(), answerDialogueText->GetTextAction()->GetText().c_str());
//	json_object_dotset_string(jsonObject, string(serializeObjectString + "AnswerUID").c_str(), answerDialogueText->GetTextAction()->GetText().c_str());

	if(destinationStep != nullptr)
		json_object_dotset_number(jsonObject, string(serializeObjectString + "DestinationStepUID").c_str(), destinationStep->GetUID());
	else
		json_object_dotset_number(jsonObject, string(serializeObjectString + "DestinationStepUID").c_str(), 0);

	int counter = 0;
	for (auto& currentCallbackAction : callbackActions)
	{
		string serializeAnswerStr = serializeObjectString + ".Answers.Answer_" + to_string(counter) + ".";
		currentCallbackAction->SaveAction(jsonObject, serializeAnswerStr);
		counter++;
	}
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

DialogueStep* StepAnswer::GetParentStep()
{
	return parentStep;
}

void StepAnswer::SetParentStep(DialogueStep* newParentStep)
{
	parentStep = newParentStep; 
}

UID StepAnswer::GetUID()
{
	return answerUID;
}

void StepAnswer::SetUID(UID newUID)
{
	answerUID = newUID; 
}
