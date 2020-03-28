#include "DialogueEditorDockPanel.h"

DialogueEditorDockPanel::DialogueEditorDockPanel(bool isVisible) : DockPanel("Dialogue Editor", isVisible)
{
	flyEngineSection = FLY_SECTION_ROOM_EDIT;
	dockPanelType = DOCK_DIALOGUE_EDITOR; 
}

DialogueEditorDockPanel::~DialogueEditorDockPanel()
{
}

bool DialogueEditorDockPanel::Draw()
{
	if (ImGui::Begin(panelName.c_str(), &visible)) 
	{

		
	}

	ImGui::End();

	return true;
}
