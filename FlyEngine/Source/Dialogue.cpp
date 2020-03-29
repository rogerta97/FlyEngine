#include "Dialogue.h"
#include "DialogueSlot.h"
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

DialogueSlot* Dialogue::AddDialogueSlot(string _stepText)
{
	// Add Slot To Dialogue -------------------
	DialogueSlot* newDialogueSlot = new DialogueSlot(_stepText);
	dialogueSlots.push_back(newDialogueSlot); 

	// Add Slot to the node graph -------------
	NodeGraph* dialoguesNodeGraph = App->moduleManager->GetCurrentDialogueEditor()->GetNodeGraph();
	
	if (dialoguesNodeGraph != nullptr)
		dialoguesNodeGraph->CreateNode(_stepText, ImVec2(0, 0)); 
	
	return newDialogueSlot; 
}
