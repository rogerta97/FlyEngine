#include "DialogueText.h"
#include "DisplayTextAction.h"

DialogueText::DialogueText()
{
	displayTextAction = new DisplayTextAction(nullptr);
	displayTextAction->displayTextBox = false;
	displayTextAction->displayTextBB = true;
}

DialogueText::~DialogueText()
{

}

void DialogueText::SetDialogueText(string newDialogueText)
{
	displayTextAction->SetText(newDialogueText); 
}

DisplayTextAction* DialogueText::GetTextAction()
{
	return displayTextAction;
}
