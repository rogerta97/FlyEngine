#include "ConsoleDockPanel.h"
#include "imgui.h"

ConsoleDockPanel::ConsoleDockPanel() : DockPanel("Console")
{
	messagesAmmount = 0; 
}

ConsoleDockPanel::~ConsoleDockPanel()
{
}

void ConsoleDockPanel::Draw()
{
	if (ImGui::Begin(panelName.c_str())){

		ImGui::Text("Im The Console Text");
		ImGui::Text("Im The Console Text");
		ImGui::Text("Im The Console Text");
		ImGui::Text("Im The Console Text");
		ImGui::Text("Im The Console Text");
		ImGui::Text("Im The Console Text");
		ImGui::Text("Im The Console Text");
		ImGui::Text("Im The Console Text");

		ImGui::End();
	}
}
