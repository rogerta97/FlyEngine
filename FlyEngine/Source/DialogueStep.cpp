#include "DialogueStep.h"
#include "DialogueText.h"
#include "DisplayTextAction.h"

DialogueStep::DialogueStep()
{
}

DialogueStep::~DialogueStep()
{
	dialogueText = new DialogueText(); 
}
