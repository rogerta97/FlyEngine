#include "DialogueViewportHandler.h"
#include "BoundingBox.h"

#include "Application.h"
#include "ModuleImGui.h"
#include "GameViewportDockPanel.h"

DialogueViewportHandler::DialogueViewportHandler()
{
	currentStep = nullptr; 
	sentenceBackgroundSquare = new BoundingBox(); 

	float ar = App->moduleImGui->gameViewportDockPanel->GetAspectRatio();
	sentenceBackgroundSquare->SetMaxPoint(float2(500 * ar, -500));
	sentenceBackgroundSquare->SetMinPoint(float2(-500 * ar, -350));
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
	sentenceBackgroundSquare->Draw(true, float4(1.0f, 1.0f, 1.0f, 0.5f));
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

void DialogueViewportHandler::SetCurrentStep(DialogueStep* newCurrentStep)
{
	currentStep = newCurrentStep; 
}
