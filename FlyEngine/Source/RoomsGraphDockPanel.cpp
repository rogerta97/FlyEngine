#include "RoomsGraphDockPanel.h"
#include "imgui.h"

RoomsGraphDockPanel::RoomsGraphDockPanel(bool isVisible) : DockPanel("Rooms Graph", isVisible)
{

}

RoomsGraphDockPanel::~RoomsGraphDockPanel()
{
}

bool RoomsGraphDockPanel::Draw()
{
	
#pragma region secutiryChecks
	if (!DockPanel::Draw())
		return false;
#pragma endregion

	if (ImGui::Begin(panelName.c_str(), &visible)) {
		ImGui::Button("Simulate Node Click");	
	}

	ImGui::End();
}
