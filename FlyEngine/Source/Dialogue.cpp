#include "Dialogue.h"
#include "DialogueStep.h"

Dialogue::Dialogue()
{
}

Dialogue::~Dialogue()
{
}

DialogueStep* Dialogue::AddDialogueStep(string _stepText)
{
	DialogueStep* newDialogueStep = new DialogueStep(_stepText);
	dialogueSteps.push_back(newDialogueStep); 
	return newDialogueStep; 
}
