#include "Dialogue.h"
#include "DialogueStep.h"

Dialogue::Dialogue()
{
}

Dialogue::~Dialogue()
{
}

DialogueStep* Dialogue::AddDialogueStep()
{
	DialogueStep* newDialogueStep = new DialogueStep(); 
	dialogueSteps.push_back(newDialogueStep); 
	return newDialogueStep; 
}
