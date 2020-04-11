#include "DialogueNodeGraph.h"
#include "Dialogue.h"
#include "DialogueStep.h"
#include "DialogueText.h"
#include "StepAnswer.h"
#include "DisplayTextAction.h"
#include "MyFileSystem.h"

#include "Application.h"
#include "ModuleInput.h"

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

	imnodes::BeginNodeEditor();

	if (dialogue != nullptr)
	{
		// Draw Nodes 
		for (auto& currentStep : dialogue->GetDialogueSteps())
		{
			imnodes::BeginNode(currentStep->GetUID());

			imnodes::PushColorStyle(imnodes::ColorStyle_TitleBar, IM_COL32(150, 80, 191, 255));

			imnodes::BeginNodeTitleBar();
			ImGui::Text("%s", currentStep->GetTextStr().c_str());
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


		// Draw Links
		for (int i = 0; i < links.size(); ++i)
		{
			const std::pair<int, int> p = links[i];

			// in this case, we just use the array index of the link
			// as the unique identifier

			imnodes::Link(i, p.first, p.second);
		}
	}

	imnodes::EndNodeEditor();

	// Check new links 
	int start_attr, end_attr;
	if (imnodes::IsLinkCreated(&start_attr, &end_attr))
	{
		flog("%d", start_attr);
		flog("%d", end_attr);

		if (dialogue->GetStepFromID(end_attr - 1) != nullptr)
			links.push_back(std::make_pair(start_attr, end_attr));
	}

	if (dialogue != nullptr && dialogue->needReload)
	{
		string nodeGraphPath = MyFileSystem::getInstance()->GetDialogueNodesDirectory() + to_string((int)dialogue->GetUID()) + ".ini";
		imnodes::LoadCurrentEditorStateFromIniFile(nodeGraphPath.c_str());
		dialogue->needReload = false;
	}

	if (saveData)
	{
		string nodeGraphPath = MyFileSystem::getInstance()->GetDialogueNodesDirectory() + to_string((int)dialogue->GetUID()) + ".ini";
		imnodes::SaveCurrentEditorStateToIniFile(nodeGraphPath.c_str());
		saveData = false; 
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
