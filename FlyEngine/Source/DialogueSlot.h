#ifndef _DIALOGUE_SLOT_H_
#define _DIALOGUE_SLOT_H_

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

	// Answers -----
	void AddStepAnswer(string _answerText);

private: 
	DialogueText* dialogueText; 
	list<StepAnswer*> answersList; 
};


#endif // !_DIALOGUE_STEP_H_

