#ifndef _DIALOGUE_STEP_H_
#define _DIALOGUE_STEP_H_

#include <list>

using namespace std; 

class DialogueAnswer;
class DialogueStep
{
	DialogueStep();
	~DialogueStep(); 

private: 
	list<DialogueAnswer*> answersList; 
};


#endif // !_DIALOGUE_STEP_H_

