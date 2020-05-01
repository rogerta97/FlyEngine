#include "Application.h"
#include "ModuleInput.h"
#include "ModuleRoomManager.h"
#include "RoomsGraphDockPanel.h"

#include "imgui.h"
#include "Room.h"
#include "SDL_opengl.h"
#include "mmgr.h"
#include "RoomsNodeGraph.h"
#include "NodeGraph.h"
#include "ImNodeDemo.h"

RoomsGraphDockPanel::RoomsGraphDockPanel(bool isVisible) : DockPanel("Rooms Graph", isVisible)
{
	flyEngineSection = FLY_SECTION_ROOM_GRAPH;
	dockPanelType = DOCK_ROOMS_GRAPH;

	roomsNodeGraph = new RoomsNodeGraph(); 
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

	if (ImGui::Begin(panelName.c_str(), &visible)) 
	{
		roomsNodeGraph->DrawGraph(); 
	}

	ImGui::End();
}

