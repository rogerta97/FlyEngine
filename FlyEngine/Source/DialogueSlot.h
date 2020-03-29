#ifndef _DIALOGUE_SLOT_H_
#define _DIALOGUE_SLOT_H_

#include "Globals.h"

#include <list>
#include <string>

using namespace std; 

class StepAnswer;
class DialogueText; 
class DialogueSlot
{
public: 
	DialogueSlot(string _dialogueText = "");
	~DialogueSlot(); 

	// Step Utilities ----
	void SetDialogueText(string _dialogueText); 

	// Answers -----------
	void AddStepAnswer(string _answerText);

	// Set & Get ---------
	UID GetUID() const; 

private: 
	DialogueText* dialogueText; 
	list<StepAnswer*> answersList; 

	UID slotUID; 
};


#endif // !_DIALOGUE_STEP_H_

