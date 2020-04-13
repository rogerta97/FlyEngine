#ifndef _DIALOGUE_SLOT_H_
#define _DIALOGUE_SLOT_H_

#include "Globals.h"
#include "SaveAndLoad.h" // bazokazo

#include <list>
#include <string>

using namespace std; 

enum Sentence_Display_Mode
{
	SENTENCE_DISPLAY_TOP,
	SENTENCE_DISPLAY_OVER_ANSWERS
};

class StepAnswer;
class DialogueText; 
class Dialogue; 
class DialogueStep
{
public: 
	DialogueStep(Dialogue* parentDialogue, string _dialogueText = "", string _dialogueName = "");
	~DialogueStep(); 

	void SaveStep(JSON_Object* jsonObject, string serializeObjectString);

	// Step Utilities ----
	void SetText(string _dialogueText); 
	string GetTextStr();
	StepAnswer* ListenAnswerClick(); 

	// Answers -----------
	StepAnswer* AddStepAnswer(string _answerText, string _answerName = "Name");
	list<StepAnswer*>& GetAnswersList();
	DialogueText* GetDialogueText(); 

	// Set & Get ---------
	string GetName();
	void SetName(string newName);

	Dialogue* GetParentDialogue();
	void SetParentDialogue(Dialogue* newParentStep);

	UID GetUID() const; 
	void SetUID(UID newUID); 

	bool isSelected = false; 
	bool isFirst = false; 
	Sentence_Display_Mode sentenceDisplayMode = SENTENCE_DISPLAY_TOP;

private: 
	Dialogue* parentDialogue; 
	DialogueText* dialogueText; 
	list<StepAnswer*> answersList; 

	string stepName; 
	UID stepUID; 
};


#endif // !_DIALOGUE_STEP_H_

