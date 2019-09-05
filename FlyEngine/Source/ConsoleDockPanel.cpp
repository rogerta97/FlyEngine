#include "ConsoleDockPanel.h"
#include "imgui.h"

ConsoleDockPanel::ConsoleDockPanel() : DockPanel("Console")
{
	isMemberFrom = MEMBER_BOTH; 
	messagesAmmount = 0; 
}

ConsoleDockPanel::~ConsoleDockPanel()
{
}

void ConsoleDockPanel::Draw()
{

#pragma region secutiryChecks
	DockPanel::Draw();
#pragma endregion

	if (ImGui::Begin(panelName.c_str())) {

		ImGui::Text("Im The Console Text");

		ImGui::End();
	}
}
