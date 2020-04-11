#ifndef _DIALOGUE_H_
#define _DIALOGUE_H_

#include "Globals.h"

#include <list>
#include <string>
#include <map>

using namespace std;

class DialogueStep; 
class StepAnswer; 
class Dialogue
{
public:
	Dialogue();
	~Dialogue();

	DialogueStep* AddDialogueStep(string _stepText, string _stepName = "");
	list<DialogueStep*>& GetDialogueSteps(); 

	DialogueStep* GetSelectedStep(); 
	void SetSelectedStep(UID selectedStepUID); 

//	DialogueStep* GetStepFromID(UID stepUI); 

	StepAnswer* GetAnswerFromID(UID key);
	DialogueStep* GetStepFromID(UID key);

	UID GetUID(); 

	bool needReload = false; 

	// Quick Acces ----------
	std::map<UID, StepAnswer*> answersMap; 
	std::map<UID, DialogueStep*> stepsMap; 

private: 
	list<DialogueStep*> dialogueSteps; 
	DialogueStep* selectedStep; 

	UID dialogueUID; 
};


#endif // !_DIALOGUE_STEP_H_


