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

		ImGui::Text("Im The Panel Graph");

		ImGui::End();
	}
}
