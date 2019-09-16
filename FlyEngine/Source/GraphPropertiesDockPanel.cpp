#include "imgui.h"
#include "GraphPropertiesDockPanel.h"
#include "mmgr.h"

GraphPropertiesDockPanel::GraphPropertiesDockPanel(bool isVisible) : DockPanel("Graph Properties", isVisible)
{
	flyEngineSection = FLY_SECTION_ROOM_GRAPH;
	dockPanelType = DOCK_GAME_VIEWPORT;
}

GraphPropertiesDockPanel::~GraphPropertiesDockPanel()
{
}

bool GraphPropertiesDockPanel::Draw()
{

#pragma region secutiryChecks
	if (!DockPanel::Draw())
		return false; 
#pragma endregion

	if (ImGui::Begin(panelName.c_str(), &visible)) {
		ImGui::Button("+");
		ImGui::SameLine(); ImGui::Button("-"); 

		ImGui::Separator(); 

		ImGui::Selectable("Test Selectable");
		ImGui::Selectable("Test Selectable");
		ImGui::Selectable("Test Selectable");
		ImGui::Selectable("Test Selectable");
		ImGui::Selectable("Test Selectable");
		ImGui::Selectable("Test Selectable");
		ImGui::Selectable("Test Selectable");
	}

	ImGui::End();
	return true;
}
