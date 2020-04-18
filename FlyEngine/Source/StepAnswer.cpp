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

	dialogueLink = new DialogueLink(); 

	SetName(newAnswerName.c_str());
	dialogueLink->sourceStep = _parentStep;
	dialogueLink->destinationStep = nullptr;
}

StepAnswer::~StepAnswer()
{

}

void StepAnswer::SetAnswerText(string newAnswerText)
{
	answerDialogueText->SetDialogueText(newAnswerText); 
}

void StepAnswer::SaveAnswer(JSON_Object* jsonObject, string serializeObjectString)
{
	json_object_dotset_string(jsonObject, string(serializeObjectString + "Name").c_str(), answerName.c_str());
	json_object_dotset_string(jsonObject, string(serializeObjectString + "Text").c_str(), answerDialogueText->GetTextAction()->GetText().c_str());
	json_object_dotset_number(jsonObject, string(serializeObjectString + "AnswerUID").c_str(), GetUID());

	// Save Link Info
	json_object_dotset_number(jsonObject, string(serializeObjectString + "Link.LinkUID").c_str(), dialogueLink->linkUID);
	json_object_dotset_number(jsonObject, string(serializeObjectString + "Link.startPinUID").c_str(), dialogueLink->startPinUID);
	json_object_dotset_number(jsonObject, string(serializeObjectString + "Link.endPinUID").c_str(), dialogueLink->endPinUID);

	if(dialogueLink->destinationStep != nullptr)
		json_object_dotset_number(jsonObject, string(serializeObjectString + "DestinationStepUID").c_str(), dialogueLink->destinationStep->GetUID());
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

void StepAnswer::CleanUp()
{
}

DialogueText* StepAnswer::GetAnswerDialogueText()
{
	return answerDialogueText;
}

DialogueStep* StepAnswer::GetDestinationStep()
{
	return dialogueLink->destinationStep;
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
	return dialogueLink->sourceStep;
}

void StepAnswer::SetParentStep(DialogueStep* newParentStep)
{
	dialogueLink->sourceStep = newParentStep;
}

UID StepAnswer::GetUID()
{
	return answerUID;
}

void StepAnswer::SetUID(UID newUID)
{
	answerUID = newUID; 
}

DialogueLink* StepAnswer::GetLink()
{
	return dialogueLink;
}

void StepAnswer::DeleteLink()
{
	if (dialogueLink != nullptr)
	{
		dialogueLink->sourceStep = nullptr; 
		dialogueLink->destinationStep = nullptr; 
		dialogueLink->linkUID = 0; 

		delete dialogueLink; 
		dialogueLink = nullptr;
	}
}

DialogueLink::~DialogueLink()
{
	linkUID = RandomNumberGenerator::getInstance()->GenerateUID(); 
}

DialogueLink::DialogueLink()
{
}
