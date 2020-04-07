#ifndef _DIALOGUE_SLOT_H_
#define _DIALOGUE_SLOT_H_

#include "Globals.h"

#include <list>
#include <string>

using namespace std; 

class StepAnswer;
class DialogueText; 
class DialogueStep
{
public: 
	DialogueStep(string _dialogueText = "");
	~DialogueStep(); 

	// Step Utilities ----
	void SetStepText(string _dialogueText); 
	string GetStepTextStr();

	// Answers -----------
	void AddStepAnswer(string _answerText);
	list<StepAnswer*>& GetAnswersList();

	// Set & Get ---------
	UID GetUID() const; 

private: 
	DialogueText* dialogueText; 
	list<StepAnswer*> answersList; 

	UID stepUID; 
};


#endif // !_DIALOGUE_STEP_H_

