#include "FileBrowserDockPanel.h"
#include "MyFileSystem.h"
#include "ResourceManager.h"
#include "Texture.h"
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
		Texture* folderTexture = (Texture*)ResourceManager::getInstance()->GetResource("FolderIcon"); 

		ImGui::Image((ImTextureID)folderTexture->GetTextureID(), ImVec2(20, 20)); ImGui::SameLine();
		ImGui::SetCursorPosX(ImGui::GetCursorPosX() - 3);
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
		string resourceName = fileName; 
		MyFileSystem::getInstance()->DeleteFileExtension(resourceName);
		Resource* currentResource = ResourceManager::getInstance()->GetResource(resourceName.c_str());
		ImTextureID iconTextureID = 0;

		switch (MyFileSystem::getInstance()->GetFileExtension(fileName))
		{
		case FILE_PNG: 
		{
			Texture* pngTexture = (Texture*)ResourceManager::getInstance()->GetResource("PNGIcon");
			iconTextureID = (ImTextureID)pngTexture->GetTextureID(); 
			break;
		}
		case FILE_JPG:
		{
			Texture* jpgTexture = (Texture*)ResourceManager::getInstance()->GetResource("JPGIcon");
			iconTextureID = (ImTextureID)jpgTexture->GetTextureID();
			break;
		}
		}

		ImGui::Image(iconTextureID, ImVec2(20, 20)); ImGui::SameLine();
		
		bool selected = false; 
		if (selectedResourceUID == currentResource->GetUID())
			selected = true; 

		if (ImGui::MenuItem(fileName.c_str(), "", &selected))
		{
			MyFileSystem::getInstance()->DeleteFileExtension(fileName);
			selectedResourceUID = currentResource->GetUID();
		}
	}
}
