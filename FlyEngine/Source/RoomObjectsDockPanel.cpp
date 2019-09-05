#include "RoomObjectsDockPanel.h"
#include "imgui.h"

RoomObjectsDockPanel::RoomObjectsDockPanel(bool isVisible) : DockPanel("Room Objects", isVisible)
{
}

RoomObjectsDockPanel::~RoomObjectsDockPanel()
{
}

bool RoomObjectsDockPanel::Draw()
{

#pragma region secutiryChecks
	if (!DockPanel::Draw())
		return false;
#pragma endregion

	if (ImGui::Begin(panelName.c_str(), &visible)) {
		ImGui::Text("Im The RoomObjects Text");
	}

	ImGui::End();
	return true;
}
