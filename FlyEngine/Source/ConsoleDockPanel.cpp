#include "ConsoleDockPanel.h"
#include "Globals.h"
#include "imgui.h"

ConsoleDockPanel::ConsoleDockPanel(bool isVisible) : DockPanel("Console", isVisible)
{
	flyEngineSection = FLY_SECTION_BOTH;
	dockPanelType = DOCK_CONSOLE;

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

	if (ImGui::Begin(panelName.c_str(), &visible, ImGuiWindowFlags_NoScrollbar)) {
		
		// Debug Button
		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.75f, 0.75f, 0.75f, 1));
		if (ImGui::Button("Debug")) {

		} 
		ImGui::PopStyleColor();

		// Warnings Button
		ImGui::SameLine();
		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.8f, 0.8f, 0.4f, 1)); 
		if (ImGui::Button("Warnings")) {

		}
		ImGui::PopStyleColor(); 

		// Errors Button
		ImGui::SameLine();
		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(1.0f, 0.2f, 0.2f, 1));
		if (ImGui::Button("Errors")) {

		}
		ImGui::PopStyleColor(); 

		// Clear Button
		ImGui::SameLine();
		ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 5.0f);
		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.2f, 0.2f, 0.2f, 1));
		if (ImGui::Button("Clear")) {

		}

		ImGui::PopStyleColor(); 
		ImGui::PopStyleVar(); 

		// Messages Area
		ImGui::Separator();
		ImGui::BeginChild("ConsoleMessagesArea", ImVec2(ImGui::GetWindowContentRegionWidth(), ImGui::GetWindowHeight() - ImGui::GetCursorPosY() * 1.6));

		ImGui::Text("Messages Should Lay Here");

		ImGui::EndChild();
		ImGui::Separator();

		// Bottom Bar
		if (ImGui::Button("Search")) {

		}
	
	}

	ImGui::End();

	return true;
}
