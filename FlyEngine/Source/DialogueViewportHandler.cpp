#include "DialogueViewportHandler.h"
#include "BoundingBox.h"
#include "DialogueStep.h"
#include "DialogueText.h"
#include "DisplayTextAction.h"

#include "Application.h"
#include "ModuleImGui.h"
#include "GameViewportDockPanel.h"

DialogueViewportHandler::DialogueViewportHandler()
{
	currentStep = nullptr; 
	sentenceBackgroundSquare = new BoundingBox();
	sentencePadding = 10.0f;

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

	if (currentStep != nullptr)
	{
		DrawSentenceBackground();
		currentStep->GetDialogueText()->GetTextAction()->RenderText();
	}
}

void DialogueViewportHandler::DrawSentenceBackground()
{
	if (currentStep != nullptr)
	{
		sentenceBackgroundSquare->Draw(true, float4(0.0f, 0.0f, 0.0f, 0.5f));
	}
}

void DialogueViewportHandler::DrawAnswers()
{

}

void DialogueViewportHandler::AddaptSentenceTextBox()
{
	if (currentStep != nullptr)
	{
		currentStep->GetDialogueText()->GetTextAction()->GetTextBox()->SetMaxPoint(sentenceBackgroundSquare->GetMaxPoint() + float2(-sentencePadding, sentencePadding));
		currentStep->GetDialogueText()->GetTextAction()->GetTextBox()->SetMinPoint(sentenceBackgroundSquare->GetMinPoint() + float2(sentencePadding, -sentencePadding));
		currentStep->GetDialogueText()->GetTextAction()->CalculateOriginTextPosition(); 
	}
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
	AddaptSentenceTextBox(); 
}
