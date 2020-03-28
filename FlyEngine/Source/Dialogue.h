#ifndef _DIALOGUE_H_
#define _DIALOGUE_H_

#include <list>

using namespace std;

class DialogueStep; 
class Dialogue
{
public:
	Dialogue();
	~Dialogue();

	DialogueStep* AddDialogueStep();

private: 
	list<DialogueStep*> dialogueSteps; 
};


#endif // !_DIALOGUE_STEP_H_


