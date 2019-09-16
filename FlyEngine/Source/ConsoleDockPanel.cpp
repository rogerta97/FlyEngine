#include "ConsoleDockPanel.h"
#include "Globals.h"
#include "mmgr.h"

#include <string>

ConsoleDockPanel::ConsoleDockPanel(bool isVisible) : DockPanel("Console", isVisible)
{
	flyEngineSection = FLY_SECTION_BOTH;
	dockPanelType = DOCK_CONSOLE;

	debugMessageAmount = 0; 
	warningMessageAmount = 0;
	errorMessageAmount = 0; 

	debugMessageColor = ImVec4{ 0.5f, 0.5f, 0.5f, 1 };
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
		
		string buttonText = ""; 

		// Debug Button
		ImGui::PushStyleColor(ImGuiCol_Button, debugMessageColor);
		buttonText = "Debug (" + std::to_string(debugMessageAmount) + ')';
		if (ImGui::Button(buttonText.c_str())) {
			SetMessageVisibility(CM_DEBUG); 
		} 
		ImGui::PopStyleColor();

		// Warnings Button
		ImGui::SameLine();
		ImGui::PushStyleColor(ImGuiCol_Button, warningMessageColor);
		buttonText = "Warnings (" + std::to_string(warningMessageAmount) + ')';
		if (ImGui::Button(buttonText.c_str())) {
			SetMessageVisibility(CM_WARNING);
		}
		ImGui::PopStyleColor(); 

		// Errors Button
		ImGui::SameLine();
		ImGui::PushStyleColor(ImGuiCol_Button, errorMessageColor);
		buttonText = "Errors (" + std::to_string(errorMessageAmount) + ')';
		if (ImGui::Button(buttonText.c_str())) {
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

		ImGui::Spacing();

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
	debugMessageAmount = warningMessageAmount = errorMessageAmount = 0; 

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

	switch (messageType)
	{
		case CM_DEBUG:
			debugMessageAmount++;
			break;
		case CM_WARNING:
			warningMessageAmount++;
			break;
		case CM_ERROR:
			errorMessageAmount++;
			break;
	}

	messagesList.push_back(newMessage);
}
	

