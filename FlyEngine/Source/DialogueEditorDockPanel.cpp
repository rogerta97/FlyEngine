#include "DialogueEditorDockPanel.h"
#include "NodeGraph.h"
#include "DialogueNodeGraph.h"
#include "ViewportManager.h"

#include "Application.h"
#include "ModuleInput.h"

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
	if (!DockPanel::Draw())
		return false;

	if (ImGui::Begin(panelName.c_str(), &visible)) 
	{
		nodeGraph->DrawGraph();
	}

	if (ImGui::IsWindowFocused())
		ViewportManager::getInstance()->blockInput = true;
	
	ImGui::End();

	return true;
}

DialogueNodeGraph* DialogueEditorDockPanel::GetNodeGraph()
{
	return nodeGraph;
}
