#include "FileBrowserDockPanel.h"
#include "MyFileSystem.h"
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

		std::string imagesFolderPath = MyFileSystem::getInstance()->GetResourcesDirectory(); 
		DrawDirectoryRecursive(imagesFolderPath); 
		
		ImGui::EndChild();
	}
	ImGui::End(); 

	return true;
}

void FileBrowserDockPanel::DrawDirectoryRecursive(string& directory)
{
	std::string fileName = MyFileSystem::getInstance()->GetLastPathItem(directory, true);
	if (MyFileSystem::getInstance()->IsFolder(directory))
	{
		ImGui::Image(0, ImVec2(18, 18)); ImGui::SameLine(); 
		if(ImGui::TreeNodeEx(fileName.c_str()))
		{
			vector<string> directoryFiles; 
			MyFileSystem::getInstance()->GetFilesInDirectory(directory.c_str(),directoryFiles, true);

			for (auto& currentFile : directoryFiles)
			{
				string currentFilePath = string(currentFile);
				DrawDirectoryRecursive(currentFilePath); 
			}

			ImGui::TreePop();
		}
	}
	else
	{
		ImGui::Image(0, ImVec2(18, 18)); ImGui::SameLine();
		ImGui::MenuItem(fileName.c_str());
	}
}
