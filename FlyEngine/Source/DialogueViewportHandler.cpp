#include "DialogueViewportHandler.h"
#include "BoundingBox.h"

DialogueViewportHandler::DialogueViewportHandler()
{
	currentStep = nullptr; 
	sentenceBackgroundSquare = new BoundingBox(); 

	sentenceBackgroundSquare->SetMaxPoint(float2(500, 500));
	sentenceBackgroundSquare->SetMinPoint(float2(-500, 400));
}

DialogueViewportHandler::~DialogueViewportHandler()
{

}

void DialogueViewportHandler::DrawDialogue()
{
	DrawAnswers();
	DrawSentence(); 
}

void DialogueViewportHandler::DrawSentence()
{
	DrawSentenceBackground(); 
}

void DialogueViewportHandler::DrawSentenceBackground()
{
	sentenceBackgroundSquare->Draw(true, float4(255, 255, 255, 255)); 
}

void DialogueViewportHandler::DrawAnswers()
{

}

UID DialogueViewportHandler::GetAnswerSelected()
{
	return UID();
}

DialogueStep* DialogueViewportHandler::GetCurrentStep()
{
	return currentStep;
}

void DialogueViewportHandler::GetCurrentStep(DialogueStep* newCurrentStep)
{
	currentStep = newCurrentStep; 
}
