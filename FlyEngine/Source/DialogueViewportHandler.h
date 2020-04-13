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

	// Utility
	void AddaptSentenceTextBox(); 

	// Set & Get
	UID GetAnswerSelected();

	float GetAnswersSpacing(); 

	BoundingBox* GetSentenceBackgroundBB(); 

	DialogueStep* GetCurrentStep();
	void SetCurrentStep(DialogueStep* newCurrentStep);

private:

	float sentencePadding; 
	float answersSpacing; 
	float answersHeight; 

	BoundingBox* sentenceBackgroundBB; 
	DialogueStep* currentStep; 
};

#endif // !_DIALOGUE_VIEWPORT_HANDLER_H_

