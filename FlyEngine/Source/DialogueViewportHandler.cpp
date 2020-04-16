#include "DialogueViewportHandler.h"
#include "BoundingBox.h"
#include "DialogueStep.h"
#include "StepAnswer.h"
#include "DialogueText.h"
#include "DisplayTextAction.h"

#include "Application.h"
#include "ModuleImGui.h"
#include "GameViewportDockPanel.h"

DialogueViewportHandler::DialogueViewportHandler()
{
	currentStep = nullptr; 
	sentenceBackgroundBB = new BoundingBox();
	sentencePadding = 15.0f;
	answersSpacing = 25.0f; 
	answersHeight = 60.0f;

	float ar = App->moduleImGui->gameViewportDockPanel->GetAspectRatio();
	sentenceBackgroundBB->SetMaxPoint(float2(500 * ar, -500));
	sentenceBackgroundBB->SetMinPoint(float2(-500 * ar, -350));
	sentenceBackgroundBB->SetSquareColor(float4(0, 0, 0, 1)); 
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

		currentStep->GetDialogueText()->GetTextAction()->SetTextColor(currentStep->fontColorHold);
		currentStep->GetDialogueText()->GetTextAction()->RenderText();
	}
}

void DialogueViewportHandler::DrawSentenceBackground()
{
	if (currentStep != nullptr)
	{
		sentenceBackgroundBB->Draw(true, currentStep->backgroundColorHold);
	}
}

void DialogueViewportHandler::DrawAnswers()
{
	if (currentStep != nullptr)
	{
		int counter = 0;
		float2 pen;
		float ar = App->moduleImGui->gameViewportDockPanel->GetAspectRatio();

		for (auto& currentAnswer : currentStep->GetAnswersList())
		{
			currentAnswer->GetAnswerDialogueText()->GetTextAction()->SetTextBBColor(currentStep->answerBackgroundColorHold);
			currentAnswer->GetAnswerDialogueText()->GetTextAction()->SetTextColor(currentStep->answerFontColorHold); 
			currentAnswer->GetAnswerDialogueText()->GetTextAction()->DrawTextBoundingBox(true);
			currentAnswer->GetAnswerDialogueText()->GetTextAction()->RenderText(false, 10); 
		}
	}	
}

void DialogueViewportHandler::AddaptSentenceTextBox()
{
	if (currentStep != nullptr)
	{
		currentStep->GetDialogueText()->GetTextAction()->GetTextBox()->SetMaxPoint(sentenceBackgroundBB->GetMaxPoint() + float2(-sentencePadding, sentencePadding));
		currentStep->GetDialogueText()->GetTextAction()->GetTextBox()->SetMinPoint(sentenceBackgroundBB->GetMinPoint() + float2(sentencePadding, -sentencePadding));
		currentStep->GetDialogueText()->GetTextAction()->CalculateOriginTextPosition(); 
	}
}

UID DialogueViewportHandler::GetAnswerSelected()
{
	return UID();
}

float DialogueViewportHandler::GetAnswersSpacing()
{
	return answersSpacing;
}

BoundingBox* DialogueViewportHandler::GetSentenceBackgroundBB()
{
	return sentenceBackgroundBB;
}

DialogueStep* DialogueViewportHandler::GetCurrentStep()
{
	return currentStep;
}

void DialogueViewportHandler::SetCurrentStep(DialogueStep* newCurrentStep)
{
	currentStep = newCurrentStep; 

	if (currentStep != nullptr)
	{
		AddaptSentenceTextBox(); 
		AddaptAnswersPosition();
	}
}

void DialogueViewportHandler::AddaptAnswersPosition()
{
	int counter = 0;
	float2 pen;
	float ar = App->moduleImGui->gameViewportDockPanel->GetAspectRatio();

	for (auto& currentAnswer : currentStep->GetAnswersList())
	{
		// Calculate Text Position
		pen.x = -450 * ar;
		pen.y = 400 - (answersSpacing * counter) - (answersHeight * counter);

		float2 textBoxMinPoint = pen + float2(0, answersHeight);
		float2 textBoxMaxPoint = pen + float2(800 * ar, 0);

		DisplayTextAction* answerTextAction = currentAnswer->GetAnswerDialogueText()->GetTextAction();

		// Addapt Text Box
		answerTextAction->GetTextBox()->SetMaxPoint(textBoxMaxPoint);
		answerTextAction->GetTextBox()->SetMinPoint(textBoxMinPoint);
		answerTextAction->CalculateOriginTextPosition();

		//// Addapt Text Bounding Box
		currentAnswer->GetAnswerDialogueText()->GetTextAction()->RenderText(true);

		float2 textBBMaxPoint = answerTextAction->GetTextBB()->GetMaxPoint();
		float2 textBBMinPoint = answerTextAction->GetTextBB()->GetMinPoint();

		answerTextAction->GetTextBB()->SetMaxPoint(textBBMaxPoint + float2(10, -10));
		answerTextAction->GetTextBB()->SetMinPoint(textBBMinPoint + float2(-10, 10));

		counter++;
	}
}
