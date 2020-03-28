#include "DialogueEditorDockPanel.h"
#include "NodeGraph.h"

DialogueEditorDockPanel::DialogueEditorDockPanel(bool isVisible) : DockPanel("Dialogue Editor", isVisible)
{
	flyEngineSection = FLY_SECTION_ROOM_EDIT;
	dockPanelType = DOCK_DIALOGUE_EDITOR; 

	nodeGraph = new NodeGraph(); 
}

DialogueEditorDockPanel::~DialogueEditorDockPanel()
{
}

bool DialogueEditorDockPanel::Draw()
{
	if (ImGui::Begin(panelName.c_str(), &visible)) 
	{
		nodeGraph->DrawNodeGraph(); 
	}

	ImGui::End();

	return true;
}
