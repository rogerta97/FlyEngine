#ifndef _DIALOGUE_STEP_H_
#define _DIALOGUE_STEP_H_

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
	void SetDialogueText(string _dialogueText); 

	// Answers -----
	void AddStepAnswer(string _answerText);

private: 
	DialogueText* dialogueText; 
	list<StepAnswer*> answersList; 
};


#endif // !_DIALOGUE_STEP_H_

