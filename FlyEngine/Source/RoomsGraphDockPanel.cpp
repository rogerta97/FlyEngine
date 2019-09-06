#include "Application.h"
#include "RoomsGraphDockPanel.h"
#include "imgui.h"

RoomsGraphDockPanel::RoomsGraphDockPanel(bool isVisible) : DockPanel("Rooms Graph", isVisible)
{
	flyEngineSection = FLY_SECTION_ROOM_GRAPH;
	dockPanelType = DOCK_ROOMS_GRAPH;
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
		
		if (ImGui::Button("Simulate Node Click")) {
			App->moduleImGui->AddaptToFlySection(FLY_SECTION_ROOM_EDIT); 
		}
	}

	ImGui::End();
}
