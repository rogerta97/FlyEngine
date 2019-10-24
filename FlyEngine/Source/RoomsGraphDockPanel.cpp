#include "Application.h"
#include "ModuleInput.h"
#include "ModuleRoomManager.h"
#include "RoomsGraphDockPanel.h"

#include "imgui.h"
#include "Room.h"
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

		if(App->moduleRoomManager->GetSelectedRoom() != nullptr && App->moduleInput->GetKey(SDL_SCANCODE_DELETE)){
			App->moduleRoomManager->DeleteRoom(App->moduleRoomManager->GetSelectedRoom()->GetName()); 
		}
	}

	ImGui::End();
}

