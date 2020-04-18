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
	imnodes::PushColorStyle(imnodes::ColorStyle_LinkHovered, IM_COL32(255, 173, 51, 255));

	imnodes::BeginNodeEditor();

	if (dialogue != nullptr)
	{
		// Draw Nodes 
		DrawGraphNodes();

		// Draw Links
		DrawGraphLinks();
	}

	imnodes::EndNodeEditor();

	// Check node clicks
	if(App->moduleInput->GetMouseButton(RI_MOUSE_BUTTON_1_DOWN) == KEY_DOWN)
		HandleNodeClick();

	int nodeHovered = 0;
	int linkHovered = 0; 
	static DialogueStep* hoveredNode = nullptr;

	if (!imnodes::IsNodeHovered(&nodeHovered))
		nodeHovered = 0;

	//flog("%d", nodeHovered);
	if (App->moduleInput->GetMouseButton(RIGHT_CLICK) == KEY_DOWN && dialogue != nullptr)
	{
		if (nodeHovered != 0)
		{
			ImGui::OpenPopup("node_dialogue_popup");
			hoveredNode = dialogue->GetStepFromID(nodeHovered); 
		}
		else
		{
			ImGui::OpenPopup("empty_space_dialogue_popup");
		}
	}

	if (ImGui::BeginPopup("empty_space_dialogue_popup"))
	{
		if (ImGui::Selectable("New Node"))
		{
			DialogueStep* newStep = dialogue->AddDialogueStep("New Step Sentence", "New Step Name");
			newStep->SetUID(RandomNumberGenerator::getInstance()->GenerateUID());

			if(dialogue != nullptr)
				dialogue->stepsMap.insert(std::make_pair(newStep->GetUID(), newStep));
				
			imnodes::SetNodeScreenSpacePos(newStep->GetUID(), ImGui::GetMousePos());

			ImGui::EndPopup();
			ImGui::CloseCurrentPopup();			
		}
		else
		{
			ImGui::EndPopup(); 
		}

	}

	if (ImGui::BeginPopup("node_dialogue_popup"))
	{
		bool closePopup = false; 
		// New Answer Item
		if (ImGui::Selectable("Add New Answer"))
		{
			if (hoveredNode != nullptr)
			{
				StepAnswer* newAnswer = hoveredNode->AddStepAnswer("Answer Text", "New Answer");

				if (dialogue != nullptr)
					dialogue->answersMap.insert(std::make_pair(newAnswer->GetUID(), newAnswer));	

				closePopup = true; 
			}
		}

		// Delete Links Item 
		if (!hoveredNode->GetAnswersList().empty())
		{
			if (ImGui::BeginMenu("Delete Link"))
			{
				int counter = 0;
				for (auto currentAnswer = hoveredNode->GetAnswersList().begin(); currentAnswer != hoveredNode->GetAnswersList().end(); currentAnswer++)
				{
					string itemStr = "Delete " + (*currentAnswer)->GetAnswerDialogueText()->GetTextAction()->GetText();
					if (ImGui::MenuItem(itemStr.c_str()))
					{
						EraseGraphLink((*currentAnswer)->GetUID());
						(*currentAnswer)->DeleteLink();

						closePopup = true;
						break;
					}
			
					counter++;
				}

				ImGui::EndMenu();
			}	
		}

		if (!hoveredNode->GetAnswersList().empty())
		{
			if (ImGui::BeginMenu("Delete Answer"))
			{
				int counter = 0;
				for (auto currentAnswer = hoveredNode->GetAnswersList().begin(); currentAnswer != hoveredNode->GetAnswersList().end(); currentAnswer++)
				{
					string itemStr = "Delete " + (*currentAnswer)->GetAnswerDialogueText()->GetTextAction()->GetText();
					if (ImGui::MenuItem(itemStr.c_str()))
					{
						EraseGraphLink((*currentAnswer)->GetUID());
						//(*currentAnswer)->DeleteLink();

						hoveredNode->DeleteAnswer((*currentAnswer)->GetUID());

						closePopup = true;
						break;
					}

					counter++;
				}

				ImGui::EndMenu();
			}
		}

		ImGui::EndPopup();

		if (closePopup)
			ImGui::CloseCurrentPopup();
	}


	imnodes::PopColorStyle();
	imnodes::PopColorStyle();

	// Check new links --------------------------------------
	int start_attr, end_attr;
	if (imnodes::IsLinkCreated(&start_attr, &end_attr))
	{
		flog("%d", start_attr);
		flog("%d", end_attr);

		int stepID = end_attr - 1; 
		DialogueStep* destinationStep = dialogue->GetStepFromID(stepID);
		if (destinationStep != nullptr)
		{
			StepAnswer* stepAnswer = dialogue->GetAnswerFromID(start_attr);

			if (stepAnswer->GetDestinationStep() == nullptr)
			{
				stepAnswer->GetLink()->destinationStep = destinationStep; 
				links.push_back(std::make_pair(start_attr, end_attr));
			}
			else
			{
				FLY_ERROR("The link could not be created: answer already linked");
			}
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

void DialogueNodeGraph::DrawGraphLinks()
{
	for (int i = 0; i < links.size(); ++i)
	{
		const std::pair<int, int> p = links[i];

		// in this case, we just use the array index of the link
		// as the unique identifier

		imnodes::Link(i, p.first, p.second);


	}
}

void DialogueNodeGraph::DrawGraphNodes()
{
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

void DialogueNodeGraph::EraseGraphLink(UID start_attr)
{
	for (auto currentLink = links.begin(); currentLink != links.end(); currentLink++)
	{
		if (currentLink->first == start_attr)
		{
			links.erase(currentLink);
			break; 
		}
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
