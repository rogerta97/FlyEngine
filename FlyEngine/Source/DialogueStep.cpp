#include "DialogueStep.h"
#include "DialogueText.h"
#include "DisplayTextAction.h"
#include "DialogueViewportHandler.h"
#include "StepAnswer.h"
#include "Dialogue.h"
#include "BoundingBox.h"
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

	fontNameHold = "None";
	backgroundColorHold = float4(0, 0, 0, 1);
	fontColorHold = float4(1, 1, 1, 1);

	answerFontNameHold = "None";
	answerBackgroundColorHold = float4(0, 0, 0, 1);
	answerFontColorHold = float4(1, 1, 1, 1);
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
	json_object_dotset_string(jsonObject, string(serializeObjectString + "Visuals.FontName").c_str(), fontNameHold.c_str());

	json_object_dotset_number(jsonObject, string(serializeObjectString + "Visuals.BackgroundColor.r").c_str(), backgroundColorHold.x);
	json_object_dotset_number(jsonObject, string(serializeObjectString + "Visuals.BackgroundColor.g").c_str(), backgroundColorHold.y);
	json_object_dotset_number(jsonObject, string(serializeObjectString + "Visuals.BackgroundColor.b").c_str(), backgroundColorHold.z);
	json_object_dotset_number(jsonObject, string(serializeObjectString + "Visuals.BackgroundColor.a").c_str(), backgroundColorHold.w);

	json_object_dotset_number(jsonObject, string(serializeObjectString + "Visuals.FontColor.r").c_str(), fontColorHold.x);
	json_object_dotset_number(jsonObject, string(serializeObjectString + "Visuals.FontColor.g").c_str(), fontColorHold.y);
	json_object_dotset_number(jsonObject, string(serializeObjectString + "Visuals.FontColor.b").c_str(), fontColorHold.z);
	json_object_dotset_number(jsonObject, string(serializeObjectString + "Visuals.FontColor.a").c_str(), fontColorHold.w);


	// Save Answer Visualization Data ----------
	json_object_dotset_number(jsonObject, string(serializeObjectString + "Answers.AnswersAmount").c_str(), answersList.size());
	json_object_dotset_string(jsonObject, string(serializeObjectString + "Answers.Visuals.FontName").c_str(), answerFontNameHold.c_str());
																		 
	json_object_dotset_number(jsonObject, string(serializeObjectString + "Answers.Visuals.BackgroundColor.r").c_str(), answerBackgroundColorHold.x);
	json_object_dotset_number(jsonObject, string(serializeObjectString + "Answers.Visuals.BackgroundColor.g").c_str(), answerBackgroundColorHold.y);
	json_object_dotset_number(jsonObject, string(serializeObjectString + "Answers.Visuals.BackgroundColor.b").c_str(), answerBackgroundColorHold.z);
	json_object_dotset_number(jsonObject, string(serializeObjectString + "Answers.Visuals.BackgroundColor.a").c_str(), answerBackgroundColorHold.w);
																		
	json_object_dotset_number(jsonObject, string(serializeObjectString + "Answers.Visuals.FontColor.r").c_str(), answerFontColorHold.x);
	json_object_dotset_number(jsonObject, string(serializeObjectString + "Answers.Visuals.FontColor.g").c_str(), answerFontColorHold.y);
	json_object_dotset_number(jsonObject, string(serializeObjectString + "Answers.Visuals.FontColor.b").c_str(), answerFontColorHold.z);
	json_object_dotset_number(jsonObject, string(serializeObjectString + "Answers.Visuals.FontColor.a").c_str(), answerFontColorHold.w);															
	
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

