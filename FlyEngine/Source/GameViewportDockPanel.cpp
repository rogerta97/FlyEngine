#include "GameViewportDockPanel.h"
#include "imgui.h"

GameViewportDockPanel::GameViewportDockPanel(bool isVisible) : DockPanel("Game Viewport", isVisible)
{
}

GameViewportDockPanel::~GameViewportDockPanel()
{
}

bool GameViewportDockPanel::Draw()
{

#pragma region secutiryChecks
	if (!DockPanel::Draw())
		return false;
#pragma endregion

	if (ImGui::Begin(panelName.c_str(), &visible)) {
		ImGui::Text("Im The Game Viewport");
	}

	ImGui::End();
	return true; 
}
