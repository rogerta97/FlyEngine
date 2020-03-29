#ifndef _DIALOGUE_H_
#define _DIALOGUE_H_

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

private: 
	list<DialogueStep*> dialogueSteps; 
};


#endif // !_DIALOGUE_STEP_H_


