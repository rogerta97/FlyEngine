#ifndef _DIALOGUE_TEXT_H_
#define _DIALOGUE_TEXT_H_

#include <list>
#include <string>
#include "FlyObject.h"

using namespace std;

class DialogueStep;
class DisplayTextAction;
class DialogueText
{
public:
	DialogueText();
	~DialogueText();
 
	void SetDialogueText(string newDialogueText); 

private:
	DisplayTextAction* displayTextAction; 	
};


#endif // !_DIALOGUE_STEP_H_


