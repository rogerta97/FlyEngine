#include "DialogueText.h"
#include "DisplayTextAction.h"

DialogueText::DialogueText()
{
}

DialogueText::~DialogueText()
{
	displayTextAction = new DisplayTextAction(nullptr);
}

void DialogueText::SetDialogueText(string newDialogueText)
{
	
}
