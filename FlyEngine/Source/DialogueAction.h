#ifndef _DIALOG_ACTION_H_
#define _DIALOG_ACTION_H_

#include <string>
#include "Action.h"
#include "Math/float4.h"

using namespace std;

class Dialogue; 
class DialogueStep; 
class DialogueAction : public Action
{
public:
	DialogueAction(FlyObject* _parentObject);
	~DialogueAction();

	void Update(float dt); 
	void Draw(); 
	void CleanUp();
	void DoAction(); 

	void SaveAction(JSON_Object* jsonObject, string serializeObjectString, bool literalStr = false, int actionPositionInObject = 0);
	void DrawUISettings();
	void DrawAnswersVisualSettings(DialogueStep* selectedStep);
	void DrawStepVisualSettings(DialogueStep* selectedStep);
	void DrawActionOccurenceCheckboxes();

	DialogueStep* AddDialogueStep(string _dialogueSlotText = "Text", string _dialogueStepName = "Name");
	Dialogue* GetDialogueData(); 

private: 
	bool showPreview; 
	bool dialoguePlaying; 

	bool drawFontPopup; 
	bool fontToSentence; 
	bool fontToAnswer; 

	// Dialogue Settings Var
	int sentencePlacementCombo; 
	int fontSize; 
	float4 fontColor; 

	bool showVariableConditions; 
	Dialogue* dialogue; 
};

#endif 
