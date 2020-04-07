#include "DialogueNodeGraph.h"
#include "Dialogue.h"
#include "DialogueStep.h"
#include "DialogueText.h"
#include "StepAnswer.h"
#include "DisplayTextAction.h"
#include "imgui.h"

DialogueNodeGraph::DialogueNodeGraph()
{
	dialogue = nullptr;
}

DialogueNodeGraph::~DialogueNodeGraph()
{
	
}

void DialogueNodeGraph::DrawGraph()
{
	if (dialogue != nullptr)
	{
		imnodes::BeginNodeEditor();

		for (auto& currentStep : dialogue->GetDialogueSteps())
		{
			imnodes::BeginNode(currentStep->GetUID());

			imnodes::PushColorStyle(imnodes::ColorStyle_TitleBar, IM_COL32(150, 80, 191, 255));

			imnodes::BeginNodeTitleBar();
			ImGui::Text("%s", currentStep->GetStepTextStr().c_str()); 
			imnodes::EndNodeTitleBar();

			imnodes::PopColorStyle(); 

			imnodes::BeginInputAttribute(currentStep->GetUID() + 1);
			ImGui::Text("In");
			imnodes::EndAttribute();

			// Draw Answer Pins
			int counter = 0; 
			for (auto& currentAnswer : currentStep->GetAnswersList())
			{
				string answerText = currentAnswer->GetAnswerDialogueText()->GetTextAction()->GetText();

				imnodes::BeginOutputAttribute(currentAnswer->GetUID());
				const float label_width = ImGui::CalcTextSize(answerText.c_str()).x;

				ImGui::Indent(350 - label_width);
				ImGui::Text("%s", answerText.c_str()); 

				imnodes::EndAttribute();
			}

			imnodes::EndNode();
		}

		imnodes::EndNodeEditor();
	}
}

Dialogue* DialogueNodeGraph::GetDialogueData()
{
	return dialogue;
}

void DialogueNodeGraph::AttachDialogue(Dialogue* newDialogue)
{
	dialogue = newDialogue;
}
