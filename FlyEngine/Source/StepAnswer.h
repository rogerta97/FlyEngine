#ifndef _DIALOGUE_ANSWER_H_
#define _DIALOGUE_ANSWER_H_

#include "Globals.h"
#include "SaveAndLoad.h"

#include <list>
#include <string>

using namespace std;


class DialogueStep; 
class DialogueLink
{
public:
	DialogueLink();
	~DialogueLink(); 

	UID linkUID;
	UID startPinUID;
	UID endPinUID;

	DialogueStep* destinationStep;
	DialogueStep* sourceStep;
};

class Action; 
class DialogueText; 
class ModifyVariableAction;
class StepAnswer
{
public:
	StepAnswer(DialogueStep* parentStep, string newAnswerText = "Text", string newAnswerName = "Answer Name");
	~StepAnswer();

	void SaveAnswer(JSON_Object* jsonObject, string serializeObjectString);
	void AddCallbackAction(Action* newCallbackAction); 
	void CleanUp(); 
	void DoCallbackActions(); 

	// Link ---------------
	DialogueLink* GetLink();
	void DeleteLink(); 

	// Set & Get ----------
	void SetAnswerText(string newAnswerText); 
	DialogueText* GetAnswerDialogueText();
	
	DialogueStep* GetDestinationStep();

	ModifyVariableAction* GetModifyVarEffect(); 
	void SetModifyVarEffect(ModifyVariableAction* newMofidyVarAction);

	string GetName(); 
	void SetName(string newName);

	DialogueStep* GetParentStep();
	void SetParentStep(DialogueStep* newParentStep);

	UID GetUID(); 
	void SetUID(UID newUID); 


private: 
	DialogueText* answerDialogueText; 

	DialogueLink* dialogueLink; 
	ModifyVariableAction* callbackModifyVariables;

	string answerName; 
	UID answerUID; 
};


#endif // !_DIALOGUE_STEP_H_


