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
	imnodes::PushColorStyle(imnodes::ColorStyle_TitleBar, IM_COL32(59, 131, 255, 255));

	imnodes::BeginNodeEditor();

	if (dialogue != nullptr)
	{
		// Draw Nodes 
		for (auto& currentStep : dialogue->GetDialogueSteps())
		{
			if (currentStep->isFirst)
			{
				imnodes::PushColorStyle(imnodes::ColorStyle_TitleBar, IM_COL32(247, 40, 40, 255));
				imnodes::PushColorStyle(imnodes::ColorStyle_TitleBarHovered, IM_COL32(255, 80, 80, 255));
				imnodes::PushColorStyle(imnodes::ColorStyle_TitleBarSelected, IM_COL32(255, 80, 80, 255));
			}

			imnodes::BeginNode(currentStep->GetUID());
	
			imnodes::BeginNodeTitleBar();
			ImGui::Text("%s", currentStep->GetTextStr().c_str());
			imnodes::EndNodeTitleBar();

			if (!currentStep->isFirst)
			{
				imnodes::BeginInputAttribute(currentStep->GetUID() + 1);
				ImGui::Text("In");
				imnodes::EndAttribute();
			}

			// Draw Answer Pins
			DrawAnswerPins(currentStep);

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

	// Check node clicks

	if(App->moduleInput->GetMouseButton(RI_MOUSE_BUTTON_1_DOWN) == KEY_DOWN)
		HandleNodeClick();

	imnodes::PopColorStyle();

	// Check new links --------------------------------------
	int start_attr, end_attr;
	if (imnodes::IsLinkCreated(&start_attr, &end_attr))
	{
		flog("%d", start_attr);
		flog("%d", end_attr);

		DialogueStep* destinationStep = dialogue->GetStepFromID(end_attr - 1);
		if (destinationStep != nullptr)
		{
			StepAnswer* stepAnswer = dialogue->GetAnswerFromID(start_attr);
			stepAnswer->SetDestinationStep(destinationStep); 
			links.push_back(std::make_pair(start_attr, end_attr));
		}
	}

	// Load Node Graph If Needed -----------------------------
	if (dialogue != nullptr && dialogue->needReload)
	{
		string nodeGraphPath = MyFileSystem::getInstance()->GetDialogueNodesDirectory() + to_string((int)dialogue->GetUID()) + ".ini";
		imnodes::LoadCurrentEditorStateFromIniFile(nodeGraphPath.c_str());
		dialogue->needReload = false;
	}

	// Save Node Graph If Needed -----------------------------
	if (saveData || App->moduleInput->GetKey(SDL_SCANCODE_0))
	{
		string nodeGraphPath = MyFileSystem::getInstance()->GetDialogueNodesDirectory() + to_string((int)dialogue->GetUID()) + ".ini";
		imnodes::SaveCurrentEditorStateToIniFile(nodeGraphPath.c_str());
		saveData = false; 
	}
}

void DialogueNodeGraph::HandleNodeClick()
{
	int num_selected_nodes = imnodes::NumSelectedNodes();
	if (num_selected_nodes > 0)
	{
		std::vector<int> selected_nodes_id_list;
		selected_nodes_id_list.resize(num_selected_nodes);
		imnodes::GetSelectedNodes(selected_nodes_id_list.data());

		for (auto& selectedNodeID : selected_nodes_id_list)
		{
			DialogueStep* selectedDialogueStep = this->GetDialogueData()->GetStepFromID(selectedNodeID);

			if (selectedDialogueStep != nullptr)
			{
				dialogue->SetSelectedStep(selectedNodeID);		
				num_selected_nodes = 0;
				return;
			}
		}
	}
}

void DialogueNodeGraph::DrawAnswerPins(DialogueStep* currentStep)
{
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

	if (currentStep->isFirst)
	{
		imnodes::PopColorStyle();
		imnodes::PopColorStyle();
		imnodes::PopColorStyle();
	}
}

Dialogue* DialogueNodeGraph::GetDialogueData()
{
	return dialogue;
}

void DialogueNodeGraph::AttachDialogue(Dialogue* newDialogue)
{
	dialogue = newDialogue;
	GenerateLinksFromDialogueAttached(); 
	dialogue->needReload = true; 
}

void DialogueNodeGraph::GenerateLinksFromDialogueAttached()
{
	for (auto& currentStep : dialogue->GetDialogueSteps())
	{
		for (auto& currentAnswer : currentStep->GetAnswersList())
		{
			if (currentAnswer->GetDestinationStep() != nullptr)
			{
				links.push_back(std::make_pair(currentAnswer->GetUID(), currentAnswer->GetDestinationStep()->GetUID() + 1));
			}
		}
	}
}
