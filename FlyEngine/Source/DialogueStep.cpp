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

void DialogueStep::SaveStep(JSON_Object* jsonObject, string serializeObjectString)
{
	json_object_dotset_string(jsonObject, string(serializeObjectString + "Name").c_str(), stepName.c_str());
	json_object_dotset_string(jsonObject, string(serializeObjectString + "Text").c_str(), dialogueText->GetTextAction()->GetText().c_str());

	json_object_dotset_number(jsonObject, string(serializeObjectString + "Answers.AnswersAmount").c_str(), answersList.size());
	
	int counter = 0; 
	for (auto& currentAnswer : answersList)
	{
		string serializeAnswerStr = serializeObjectString + "Answers.Answer_" + to_string(counter) + ".";
		currentAnswer->SaveAnswer(jsonObject, serializeAnswerStr);
		counter++; 
	}
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
