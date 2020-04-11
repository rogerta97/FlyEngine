#ifndef _DIALOGUE_ANSWER_H_
#define _DIALOGUE_ANSWER_H_

#include "Globals.h"
#include "SaveAndLoad.h"

#include <list>
#include <string>

using namespace std;

class Action; 
class DialogueStep; 
class DialogueText; 

class StepAnswer
{
public:
	StepAnswer(DialogueStep* parentStep, string newAnswerText = "Text", string newAnswerName = "Answer Name");
	~StepAnswer();

	void SaveAnswer(JSON_Object* jsonObject, string serializeObjectString);
	void AddCallbackAction(Action* newCallbackAction); 

	// Set & Get ----------
	void SetAnswerText(string newAnswerText); 
	DialogueText* GetAnswerDialogueText();
	
	void SetDestinationStep(DialogueStep* dstStep);
	DialogueStep* GetDestinationStep();

	string GetName(); 
	void SetName(string newName);

	DialogueStep* GetParentStep();
	void SetParentStep(DialogueStep* newParentStep);

	UID GetUID(); 
	void SetUID(UID newUID); 

private: 

	DialogueText* answerDialogueText; 

	DialogueStep* destinationStep; 
	DialogueStep* parentStep; 

	list<Action*> callbackActions;

	string answerName; 
	UID answerUID; 
};


#endif // !_DIALOGUE_STEP_H_


