#include "ConsoleDockPanel.h"
#include "Globals.h"
#include "mmgr.h"

ConsoleDockPanel::ConsoleDockPanel(bool isVisible) : DockPanel("Console", isVisible)
{
	flyEngineSection = FLY_SECTION_BOTH;
	dockPanelType = DOCK_CONSOLE;

	printDebugMessages = true; 
	printWarningMessages = true;
	printErrorMessages = true;

	debugMessageColor = ImVec4{ 0.4f, 0.4f, 0.4f, 1 };
	warningMessageColor = ImVec4{ 0.9f, 0.6f, 0.2f, 1 };
	errorMessageColor = ImVec4{ 1.0f, 0.2f, 0.2f, 1 };
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
		ImGui::PushStyleColor(ImGuiCol_Button, debugMessageColor);
		if (ImGui::Button("Debug")) {
			SetMessageVisibility(CM_DEBUG); 
		} 
		ImGui::PopStyleColor();

		// Warnings Button
		ImGui::SameLine();
		ImGui::PushStyleColor(ImGuiCol_Button, warningMessageColor);
		if (ImGui::Button("Warnings")) {
			SetMessageVisibility(CM_WARNING);
		}
		ImGui::PopStyleColor(); 

		// Errors Button
		ImGui::SameLine();
		ImGui::PushStyleColor(ImGuiCol_Button, errorMessageColor);
		if (ImGui::Button("Errors")) {
			SetMessageVisibility(CM_ERROR);
		}
		ImGui::PopStyleColor(); 

		// Clear Button
		ImGui::SameLine();
		ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 5.0f);
		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.75f, 0.75f, 0.75f, 1));
		if (ImGui::Button("Clear")) {
			CleanUp(); 
		}

		ImGui::PopStyleColor(); 
		ImGui::PopStyleVar(); 

		// Messages Area
		ImGui::Separator();
		ImGui::BeginChild("ConsoleMessagesArea", ImVec2(ImGui::GetWindowContentRegionWidth(), ImGui::GetWindowHeight() - ImGui::GetCursorPosY() - 30));

		PrintMessageStack(); 

		ImGui::EndChild();
		ImGui::Separator();

		// Bottom Bar
		if (ImGui::Button("Search")) {

		}
	
		// Command Bar
		ImGui::SameLine(); 
		ImGui::InputText("", commandBuffer, IM_ARRAYSIZE(commandBuffer));
	
	}

	ImGui::End();

	return true;
}

bool ConsoleDockPanel::CleanUp()
{
	for (auto it = messagesList.begin(); it != messagesList.end(); it++) {
		delete (*it); 
	}

	messagesList.clear(); 

	return true; 
}

void ConsoleDockPanel::PrintMessageStack()
{
	for (auto it = messagesList.begin(); it != messagesList.end(); it++) {

		if ((*it)->canPrint) {

			switch ((*it)->messageType)
			{
			case CM_DEBUG:
				ImGui::TextColored(debugMessageColor, (*it)->message.c_str());
				break;

			case CM_WARNING:
				ImGui::TextColored(warningMessageColor, (*it)->message.c_str());
				break;

			case CM_ERROR:
				ImGui::TextColored(errorMessageColor, (*it)->message.c_str());
				break;

			default:
				break;
			}
		}	
	}
}

void ConsoleDockPanel::SetMessageVisibility(consoleMessageType messageType)
{
	for (auto it = messagesList.begin(); it != messagesList.end(); it++) {

		if ((*it)->messageType == messageType) 		{
			(*it)->canPrint = !(*it)->canPrint;
		}
	}
}

void ConsoleDockPanel::AddLineToConsole(consoleMessageType messageType, string textToAdd)
{
	ConsoleMessage* newMessage = new ConsoleMessage();

	newMessage->message = textToAdd;
	newMessage->messageType = messageType;
	newMessage->canPrint = true;

	messagesList.push_back(newMessage);
}
	

