#include "DialogueStep.h"
#include "DialogueText.h"
#include "DisplayTextAction.h"
#include "StepAnswer.h"
#include "Dialogue.h"
#include "NodeGraph.h"

#include "Application.h"
#include "ModuleManager.h"
#include "ModuleInput.h"
#include "DialogueEditorDockPanel.h"

DialogueStep::DialogueStep(Dialogue* _parentDialogue, string _dialogueText, string _dialogueName)
{
	dialogueText = new DialogueText();
	dialogueText->SetDialogueText(_dialogueText);
	stepUID = RandomNumberGenerator::getInstance()->GenerateUID(); 
	parentDialogue = _parentDialogue; 
	SetName(_dialogueName.c_str());
}

DialogueStep::~DialogueStep()
{
	
}

void DialogueStep::SaveStep(JSON_Object* jsonObject, string serializeObjectString)
{
	// Save Step Data ----------
	json_object_dotset_string(jsonObject, string(serializeObjectString + "Name").c_str(), stepName.c_str());
	json_object_dotset_string(jsonObject, string(serializeObjectString + "Text").c_str(), dialogueText->GetTextAction()->GetText().c_str());
	json_object_dotset_number(jsonObject, string(serializeObjectString + "StepID").c_str(), GetUID());
	json_object_dotset_boolean(jsonObject, string(serializeObjectString + "First").c_str(), isFirst);

	// Save Step Visualization Data ----------
	json_object_dotset_string(jsonObject, string(serializeObjectString + "Visuals.FontName").c_str(), stepName.c_str());
	json_object_dotset_string(jsonObject, string(serializeObjectString + "Visuals.BackgroundColor").c_str(), stepName.c_str());
	json_object_dotset_string(jsonObject, string(serializeObjectString + "Visuals.FontColor").c_str(), stepName.c_str());

	json_object_dotset_number(jsonObject, string(serializeObjectString + "Answers.AnswersAmount").c_str(), answersList.size());
	
	int counter = 0; 
	for (auto& currentAnswer : answersList)
	{
		string serializeAnswerStr = serializeObjectString + "Answers.Answer_" + to_string(counter) + ".";
		currentAnswer->SaveAnswer(jsonObject, serializeAnswerStr);
		counter++; 
	}
}

void DialogueStep::SetText(string _dialogueText)
{
	dialogueText->SetDialogueText(_dialogueText);
}

string DialogueStep::GetTextStr()
{
	if (dialogueText != nullptr)
	{
		return dialogueText->GetTextAction()->GetText();
	}

	return string();
}

StepAnswer* DialogueStep::ListenAnswerClick()
{
	for (auto& currentAnswer : answersList)
	{
		if (currentAnswer->GetAnswerDialogueText()->GetTextAction()->GetTextBB()->IsBoxClicked())
			return currentAnswer;
	}

	return nullptr; 
}

StepAnswer* DialogueStep::AddStepAnswer(string _answerText, string _answerName)
{
	// Add Answer ----
	StepAnswer* newStepAnswer = new StepAnswer(this); 
	newStepAnswer->SetAnswerText(_answerText);
	answersList.push_back(newStepAnswer); 

	//parentDialogue->answersMap.insert(std::make_pair(newStepAnswer->GetUID(), newStepAnswer));

	return newStepAnswer; 
}

list<StepAnswer*>& DialogueStep::GetAnswersList()
{
	return answersList;
}

DialogueText* DialogueStep::GetDialogueText()
{
	return dialogueText;
}

string DialogueStep::GetName()
{
	return stepName;
}

void DialogueStep::SetName(string newName)
{
	stepName = newName; 
}

Dialogue* DialogueStep::GetParentDialogue()
{
	return parentDialogue;
}

void DialogueStep::SetParentDialogue(Dialogue* newParentDialogue)
{
	parentDialogue = newParentDialogue; 
}

UID DialogueStep::GetUID() const
{
	return stepUID;
}

void DialogueStep::SetUID(UID newUID)
{
	stepUID = newUID; 
}
