#ifndef _DIALOGUE_H_
#define _DIALOGUE_H_

#include "Globals.h"

#include <list>
#include <string>

using namespace std;

class DialogueStep; 
class Dialogue
{
public:
	Dialogue();
	~Dialogue();

	DialogueStep* AddDialogueStep(string _stepText);
	list<DialogueStep*>& GetDialogueSteps(); 

	DialogueStep* GetSelectedStep(); 
	void SetSelectedStep(UID selectedStepUID); 

	DialogueStep* GetStepFromID(UID stepUI); 

private: 
	list<DialogueStep*> dialogueSteps; 
	DialogueStep* selectedStep; 
};


#endif // !_DIALOGUE_STEP_H_


