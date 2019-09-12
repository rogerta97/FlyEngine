#include "imgui.h"
#include "ProjectInfoDockPanel.h"
#include "mmgr.h"

ProjectInfoDockPanel::ProjectInfoDockPanel(bool isVisible) : DockPanel("Project Info", isVisible)
{
	flyEngineSection = FLY_SECTION_ROOM_GRAPH;
	dockPanelType = DOCK_GAME_VIEWPORT;
}

ProjectInfoDockPanel::~ProjectInfoDockPanel()
{
}

bool ProjectInfoDockPanel::Draw()
{

#pragma region secutiryChecks
	if (!DockPanel::Draw())
		return false; 
#pragma endregion

	if (ImGui::Begin(panelName.c_str(), &visible)) {
		ImGui::Text("Im The Project Info");	
	}

	ImGui::End();
	return true;
}
