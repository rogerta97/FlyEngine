#include "Dialogue.h"
#include "DialogueStep.h"
#include "NodeGraph.h"

#include "Application.h"
#include "ModuleManager.h"
#include "DialogueEditorDockPanel.h"
#include "DialogueViewportHandler.h"

#include "RandomNumberGenerator.h"

#include "mmgr.h"

Dialogue::Dialogue()
{
	selectedStep = nullptr;
	dialogueUID = RandomNumberGenerator::getInstance()->GenerateUID(); 
	dialogueViewportHandler = new DialogueViewportHandler(); 
}

Dialogue::~Dialogue()
{
}

void Dialogue::Draw()
{
	if(dialogueViewportHandler != nullptr)
		dialogueViewportHandler->DrawDialogue(); 
}

DialogueStep* Dialogue::AddDialogueStep(string _stepText, string _stepName)
{
	// Add Slot To Dialogue -------------------
	DialogueStep* newDialogueStep = new DialogueStep(this, _stepText, _stepName);
	dialogueSteps.push_back(newDialogueStep); 

	if (dialogueSteps.size() == 1)
	{
		newDialogueStep->GetParentDialogue()->SetFirstStep(newDialogueStep); 
	}
	 
	return newDialogueStep; 
}

list<DialogueStep*>& Dialogue::GetDialogueSteps()
{
	return dialogueSteps; 
}

void Dialogue::DeleteDialogueStep(UID stepToDeleteUID)
{
	for (auto currentStep = dialogueSteps.begin(); currentStep != dialogueSteps.end(); currentStep++)
	{
		if ((*currentStep)->GetUID() == stepToDeleteUID)
		{
			(*currentStep)->CleanUp();

			(*currentStep) == selectedStep; 
			selectedStep = nullptr; 
			
			delete (*currentStep);
			dialogueSteps.erase(currentStep);

			break; 
		}
	}
}

DialogueStep* Dialogue::GetSelectedStep()
{
	return selectedStep;
}

void Dialogue::SetSelectedStep(UID selectedStepUID)
{
	bool set = false; 
	for (auto& currentStep : dialogueSteps)
	{
		if (selectedStepUID == currentStep->GetUID())
		{
			currentStep->isSelected = true;
			selectedStep = currentStep; 
			set = true; 
		}		
		else
			currentStep->isSelected = false; 
	}

	if (!set)
		selectedStep = nullptr; 
}

UID Dialogue::GetUID()
{
	return dialogueUID;
}

//DialogueStep* Dialogue::GetStepFromID(UID stepUID)
//{
//	for (auto& currentStep : dialogueSteps)
//	{
//		if (currentStep->GetUID() == stepUID)
//			return currentStep;
//	}
//
//	return nullptr; 
//}

StepAnswer* Dialogue::GetAnswerFromID(UID key)
{	
	return answersMap[(int)key];
}

DialogueStep* Dialogue::GetStepFromID(UID key)
{
	return stepsMap[(int)key]; 	
}

DialogueStep* Dialogue::GetFirstStep()
{
	return firstStep;
}

void Dialogue::SetFirstStep(DialogueStep* firstStep)
{
	for (auto& currentStep : dialogueSteps)
	{
		currentStep->isFirst = false; 

		if (currentStep->GetUID() == firstStep->GetUID())
		{
			this->firstStep = currentStep; 
			currentStep->isFirst = true;
		}
	}	
}
