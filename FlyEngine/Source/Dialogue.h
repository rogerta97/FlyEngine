#ifndef _DIALOGUE_H_
#define _DIALOGUE_H_

#include <list>

using namespace std;

class DialogueStep; 
class Dialogue
{
	Dialogue();
	~Dialogue();

private: 
	list<DialogueStep*> dialogueSteps; 
};


#endif // !_DIALOGUE_STEP_H_


