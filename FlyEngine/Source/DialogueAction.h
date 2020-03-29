#ifndef _DIALOG_ACTION_H_
#define _DIALOG_ACTION_H_

#include <string>
#include "Action.h"

using namespace std;

class Dialogue; 
class DialogueSlot; 
class DialogueAction : public Action
{
public:
	DialogueAction(FlyObject* _parentObject);
	~DialogueAction();

	void SaveAction(JSON_Object* jsonObject, string serializeObjectString, bool literalStr = false, int actionPositionInObject = 0);
	void DrawUISettings();

	DialogueSlot* AddDialogueSlot(string _dialogueSlotText = "");

private: 
	Dialogue* dialogue; 
};

#endif 
