#include "GraphPropertiesDockPanel.h"
#include "Application.h"
#include "ModuleImGui.h"
#include "ModuleInput.h"
#include "ModuleRoomManager.h"
#include "Room.h"
#include "NodeGraph.h"
#include "imgui.h"

#include "mmgr.h"
#include <string>

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

		if(ImGui::TreeNode("Graph Settings")) {

		}
	}

	ImGui::End();
	return true;
}


