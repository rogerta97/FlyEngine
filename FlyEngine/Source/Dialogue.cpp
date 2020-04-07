#include "Dialogue.h"
#include "DialogueStep.h"
#include "NodeGraph.h"

#include "Application.h"
#include "ModuleManager.h"
#include "DialogueEditorDockPanel.h"

Dialogue::Dialogue()
{
}

Dialogue::~Dialogue()
{
}

DialogueStep* Dialogue::AddDialogueStep(string _stepText)
{
	// Add Slot To Dialogue -------------------
	DialogueStep* newDialogueStep = new DialogueStep(_stepText);
	dialogueSteps.push_back(newDialogueStep); 

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
