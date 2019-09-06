#include "ConsoleDockPanel.h"
#include "imgui.h"

ConsoleDockPanel::ConsoleDockPanel(bool isVisible) : DockPanel("Console", isVisible)
{
	flyEngineSection = FLY_SECTION_BOTH;
	dockPanelType = DOCK_CONSOLE; 
	messagesAmmount = 0; 
}

ConsoleDockPanel::~ConsoleDockPanel()
{
}

bool ConsoleDockPanel::Draw()
{

#pragma region secutiryChecks
	if (!DockPanel::Draw())
		return false;
#pragma endregion

	if (ImGui::Begin(panelName.c_str(), &visible)) {
		ImGui::Text("Im The Console Text");
	}

	ImGui::End();

	return true;
}
