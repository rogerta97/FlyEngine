#include "Application.h"
#include "ModuleInput.h"
#include "ModuleWorldManager.h"
#include "RoomsGraphDockPanel.h"

#include "imgui.h"
#include "SDL_opengl.h"
#include "mmgr.h"

#include "NodeGraph.h"
#include "ImNodeDemo.h"

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
		NodeGraph::Update();
		HandlePopups(); 
	}

	ImGui::End();
}

void RoomsGraphDockPanel::HandlePopups()
{
	NewRoomPopupHandler(); 
}

void RoomsGraphDockPanel::NewRoomPopupHandler()
{
	if (App->moduleInput->GetMouseButton(RIGHT_CLICK) == KEY_DOWN) {
		ImGui::OpenPopup("new_room_popup");
	}

	if (ImGui::BeginPopup("new_room_popup"))
	{
		if (ImGui::Selectable("Add Empty Room")) {
			App->moduleWorldManager->CreateEmptyRoom();
		}

		ImGui::EndPopup();
	}
}
