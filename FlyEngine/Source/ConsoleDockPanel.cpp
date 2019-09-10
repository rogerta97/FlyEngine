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

	if (ImGui::Begin(panelName.c_str(), &visible)) {
		
		if (ImGui::Button("Debug")) {

		} 

		ImGui::SameLine(); 
		if (ImGui::Button("Warnings")) {

		}

		ImGui::SameLine();
		if (ImGui::Button("Errors")) {

		}

		ImGui::SameLine();
		if (ImGui::Button("Clear")) {

		}

		ImGui::Separator();

		ImGui::BeginChild("ConsoleMessagesArea", ImVec2(ImGui::GetWindowWidth() - 100, ImGui::GetWindowHeight()), true); 

		ImGui::Text("Messages Should Lay Here");

		ImGui::EndChild();

		ImGui::Separator();

		if (ImGui::Button("Search")) {

		}

		//ImGui::SameLine(); 

		//ImGui::InputText("SearchFilterBox", );
	
	}

	ImGui::End();

	return true;
}
