#include "imgui.h"
#include "ProjectInfoDockPanel.h"

ProjectInfoDockPanel::ProjectInfoDockPanel(bool isVisible) : DockPanel("Project Info", isVisible)
{
	isMemberFrom = MEMBER_BOTH;
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
