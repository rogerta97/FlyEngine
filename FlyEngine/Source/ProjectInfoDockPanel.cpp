#include "RoomGraphDockPanel.h"
#include "imgui.h"
#include "ProjectInfoDockPanel.h"

ProjectInfoDockPanel::ProjectInfoDockPanel() : DockPanel("Project Info")
{
	isMemberFrom = MEMBER_BOTH;
}

ProjectInfoDockPanel::~ProjectInfoDockPanel()
{
}

void ProjectInfoDockPanel::Draw()
{

#pragma region secutiryChecks
	DockPanel::Draw();
#pragma endregion

	if (ImGui::Begin(panelName.c_str())) {

		ImGui::Text("Im The Info");

		ImGui::End();
	}
}
