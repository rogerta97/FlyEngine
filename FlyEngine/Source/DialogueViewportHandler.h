#ifndef _DIALOGUE_VIEWPORT_HANDLER_H_
#define _DIALOGUE_VIEWPORT_HANDLER_H_

#include "Globals.h"

class BoundingBox; 
class DialogueStep; 
class DialogueViewportHandler
{
public: 
	DialogueViewportHandler();
	~DialogueViewportHandler(); 

	// Draw Dialogue
	void DrawDialogue(); 

	// Draw Sentence 
	void DrawSentence();
	void DrawSentenceBackground(); 

	// Draw Answers 
	void DrawAnswers(); 

	// Set & Get
	UID GetAnswerSelected();

	DialogueStep* GetCurrentStep();
	void GetCurrentStep(DialogueStep* newCurrentStep);

private:
	BoundingBox* sentenceBackgroundSquare; 
	DialogueStep* currentStep; 
};

#endif // !_DIALOGUE_VIEWPORT_HANDLER_H_

