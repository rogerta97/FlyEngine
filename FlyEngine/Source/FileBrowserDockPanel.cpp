#include "FileBrowserDockPanel.h"

#include "imgui.h"

#include "mmgr.h"

FileBrowserDockPanel::FileBrowserDockPanel(bool isVisible) : DockPanel("Resources", isVisible)
{
	flyEngineSection = FLY_SECTION_ROOM_EDIT;
	dockPanelType = DOCK_FILE_BROWSER;
}

FileBrowserDockPanel::~FileBrowserDockPanel()
{

}

bool FileBrowserDockPanel::Draw()
{
	if (ImGui::Begin(panelName.c_str(), &visible)) {
		
		ImGui::Columns(2, NULL, true);
		ImGui::SetColumnWidth(0, 225);
		
		ImGui::BeginChild("BrowserPreview", ImVec2(210, 250), true);
		ImGui::EndChild();

		ImGui::NextColumn();	

		ImGui::BeginChild("BrowserFiles", ImVec2(ImGui::GetContentRegionMax().x, ImGui::GetContentRegionMax().y - 40));
		ImGui::EndChild();

	}
	ImGui::End(); 

	return true;
}
