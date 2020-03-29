#ifndef _DIALOGUE_ANSWER_H_
#define _DIALOGUE_ANSWER_H_

#include <list>
#include <string>

using namespace std;

class Action; 
class DialogueStep; 
class DialogueText; 
class StepAnswer
{
public:
	StepAnswer(string newAnswerText = "");
	~StepAnswer();

	void AddCallbackAction(Action* newCallbackAction); 

	// Set & Get ----------
	void SetAnswerText(string newAnswerText); 
	DialogueText* GetAnswerDialogueText();
	
	void SetDestinationStep(DialogueStep* dstStep);
	DialogueStep* GetDestinationStep();

private: 
	DialogueText* answerDialogueText; 
	DialogueStep* destinationStep; 
	list<Action*> callbackActions;
};


#endif // !_DIALOGUE_STEP_H_


