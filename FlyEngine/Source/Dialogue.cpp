#include "Dialogue.h"
#include "DialogueStep.h"
#include "NodeGraph.h"

#include "Application.h"
#include "ModuleManager.h"
#include "DialogueEditorDockPanel.h"

#include "RandomNumberGenerator.h"

#include "mmgr.h"

Dialogue::Dialogue()
{
	selectedStep = nullptr;
	dialogueUID = RandomNumberGenerator::getInstance()->GenerateUID(); 
}

Dialogue::~Dialogue()
{
}

DialogueStep* Dialogue::AddDialogueStep(string _stepText, string _stepName)
{
	// Add Slot To Dialogue -------------------
	DialogueStep* newDialogueStep = new DialogueStep(this, _stepText, _stepName);
	dialogueSteps.push_back(newDialogueStep); 
	stepsMap.insert(std::make_pair(newDialogueStep->GetUID(), newDialogueStep));

	//// Add Slot to the node graph -------------
	//NodeGraph* dialoguesNodeGraph = App->moduleManager->GetCurrentDialogueEditor()->GetNodeGraph();
	//
	//if (dialoguesNodeGraph != nullptr)
	//	dialoguesNodeGraph->CreateNode(_stepText, ImVec2(0, 0), newDialogueSlot->GetUID()); 
	//
	return newDialogueStep; 
}

list<DialogueStep*>& Dialogue::GetDialogueSteps()
{
	return dialogueSteps; 
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
	return answersMap[key];
}

DialogueStep* Dialogue::GetStepFromID(UID key)
{
	return stepsMap[key]; 	
}
