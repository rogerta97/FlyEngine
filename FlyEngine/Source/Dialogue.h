#ifndef _DIALOGUE_H_
#define _DIALOGUE_H_

#include <list>
#include <string>

using namespace std;

class DialogueSlot; 
class Dialogue
{
public:
	Dialogue();
	~Dialogue();

	DialogueSlot* AddDialogueSlot(string _stepText);

private: 
	list<DialogueSlot*> dialogueSlots; 
};


#endif // !_DIALOGUE_STEP_H_


