#include "DialogueNodeGraph.h"
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

		imnodes::BeginNode(1);
		ImGui::Dummy(ImVec2(80.0f, 45.0f));
		imnodes::EndNode();

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
