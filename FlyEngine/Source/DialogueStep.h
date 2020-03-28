#ifndef _DIALOGUE_STEP_H_
#define _DIALOGUE_STEP_H_

#include <list>
#include <string>

using namespace std; 

class DialogueAnswer;
class DialogueStep
{
public: 
	DialogueStep();
	~DialogueStep(); 

private: 
	string stepSentence; 
	list<DialogueAnswer*> answersList; 
};


#endif // !_DIALOGUE_STEP_H_

