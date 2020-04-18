#ifndef _DIALOGUE_SLOT_H_
#define _DIALOGUE_SLOT_H_

#include "Globals.h"
#include "SaveAndLoad.h" // bazokazo
#include "Math/float4.h"

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
	void CleanUp();

	void DeleteLinksAndAnswers();

	// Step Utilities ----
	void SetText(string _dialogueText); 
	string GetTextStr();
	StepAnswer* ListenAnswerClick(); 

	// Answers -----------
	StepAnswer* AddStepAnswer(string _answerText, string _answerName = "Name");
	list<StepAnswer*>& GetAnswersList();
	DialogueText* GetDialogueText(); 
	void DeleteAnswer(UID answerUID); 

	// Set & Get ---------
	string GetName();
	void SetName(string newName);

	Dialogue* GetParentDialogue();
	void SetParentDialogue(Dialogue* newParentStep);

	UID GetUID() const; 
	void SetUID(UID newUID); 

	// Public Vars
	bool isSelected = false; 
	bool isFirst = false; 
	bool styleModified = false; 

	// Draw Hold Data 
	string fontNameHold;
	float4 backgroundColorHold; 
	float4 fontColorHold; 

	Sentence_Display_Mode sentenceDisplayMode = SENTENCE_DISPLAY_TOP;

	string answerFontNameHold;
	float4 answerBackgroundColorHold;
	float4 answerFontColorHold;

private: 

	Dialogue* parentDialogue; 
	DialogueText* dialogueText; 
	list<StepAnswer*> answersList; 

	string stepName; 
	UID stepUID; 
};


#endif // !_DIALOGUE_STEP_H_

