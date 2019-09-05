#include "RoomGraphDockPanel.h"
#include "imgui.h"

RoomGraphDockPanel::RoomGraphDockPanel() : DockPanel("Room Graph")
{
	isMemberFrom = MEMBER_ROOM_OVERVIEW;
}

RoomGraphDockPanel::~RoomGraphDockPanel()
{
}

void RoomGraphDockPanel::Draw()
{

#pragma region secutiryChecks
	DockPanel::Draw();
#pragma endregion

	if (ImGui::Begin(panelName.c_str())) {

		if (ImGui::Button("Simulate Graph Click")) {

		}

		ImGui::End();
	}
}
