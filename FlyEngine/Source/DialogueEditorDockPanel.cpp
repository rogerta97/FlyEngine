#include "DialogueEditorDockPanel.h"
#include "NodeGraph.h"
#include "DialogueNodeGraph.h"

DialogueEditorDockPanel::DialogueEditorDockPanel(bool isVisible) : DockPanel("Dialogue Editor", isVisible)
{
	flyEngineSection = FLY_SECTION_ROOM_EDIT;
	dockPanelType = DOCK_DIALOGUE_EDITOR; 

	nodeGraph = new DialogueNodeGraph();
}

DialogueEditorDockPanel::~DialogueEditorDockPanel()
{
}

bool DialogueEditorDockPanel::Draw()
{
	if (ImGui::Begin(panelName.c_str(), &visible)) 
	{
		nodeGraph->DrawGraph();
	}

	ImGui::End();

	return true;
}

DialogueNodeGraph* DialogueEditorDockPanel::GetNodeGraph()
{
	return nodeGraph;
}
