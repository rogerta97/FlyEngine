#ifndef _DIALOGUE_SLOT_H_
#define _DIALOGUE_SLOT_H_

#include "Globals.h"
#include "SaveAndLoad.h" // bazokazo

#include <list>
#include <string>

using namespace std; 

class StepAnswer;
class DialogueText; 

class DialogueStep
{
public: 
	DialogueStep(string _dialogueText = "", string _dialogueName = "");
	~DialogueStep(); 

	void SaveStep(JSON_Object* jsonObject, string serializeObjectString);

	// Step Utilities ----
	void SetText(string _dialogueText); 
	string GetTextStr();

	// Answers -----------
	StepAnswer* AddStepAnswer(string _answerText, string _answerName = "Name");
	list<StepAnswer*>& GetAnswersList();

	// Set & Get ---------
	string GetName();
	void SetName(string newName);

	UID GetUID() const; 

	bool isSelected = false; 

private: 
	DialogueText* dialogueText; 
	list<StepAnswer*> answersList; 

	string stepName; 
	UID stepUID; 
};


#endif // !_DIALOGUE_STEP_H_

