#include "FileBrowserDockPanel.h"
#include "MyFileSystem.h"
#include "ResourceManager.h"
#include "ModuleImGui.h"
#include "GameViewportDockPanel.h"
#include "Texture.h"
#include "imgui.h"

#include "mmgr.h"

FileBrowserDockPanel::FileBrowserDockPanel(bool isVisible) : DockPanel("Resources", isVisible)
{
	flyEngineSection = FLY_SECTION_ROOM_EDIT;
	dockPanelType = DOCK_FILE_BROWSER;
	selectedResourceUID = 0; 
}

FileBrowserDockPanel::~FileBrowserDockPanel()
{

}

bool FileBrowserDockPanel::Draw()
{
	if (ImGui::Begin(panelName.c_str(), &visible)) {
		
		ImGui::Columns(2, NULL, true);
		ImGui::SetColumnWidth(0, 225);

		DrawLeftColumn();

		ImGui::NextColumn();	

		DrawRightColumn();
	}
	ImGui::End(); 

	return true;
}

void FileBrowserDockPanel::DrawRightColumn()
{
	ImGui::BeginChild("BrowserFiles", ImVec2(ImGui::GetContentRegionMax().x, ImGui::GetContentRegionMax().y - 40));

	std::string imagesFolderPath = MyFileSystem::getInstance()->GetResourcesDirectory();
	DrawDirectoryRecursive(imagesFolderPath);

	ImGui::EndChild();
}

void FileBrowserDockPanel::DrawLeftColumn()
{
	ImGui::BeginChild("BrowserPreview", ImVec2(210, 240), true);

	if (selectedResourceUID == 0)
	{
		ImGui::EndChild();
		return; 
	}

	Resource* selectedResource = ResourceManager::getInstance()->GetResource(selectedResourceUID);

	switch (selectedResource->GetType())
	{
	case ResourceType::RESOURCE_TEXTURE:
	{
		Texture* resourceTexture = (Texture*)selectedResource; 
		ImVec2 centerPoint = ImVec2(ImGui::GetContentRegionMax().x / 2, ImGui::GetContentRegionMax().y / 2); 
		ImVec2 imageProportions = GetImageDimensionsInPreview(resourceTexture); 

		ImGui::SetCursorPos(ImVec2(ImGui::GetCursorPosX() + centerPoint.x - ((imageProportions.x - 5) / 2), ImGui::GetCursorPosY() + centerPoint.y - ((imageProportions.y - 5) / 2)));
		ImGui::Image((ImTextureID)resourceTexture->GetTextureID(), ImVec2(imageProportions.x - 8, imageProportions.y - 10));

		ImGui::EndChild();

		DrawResourceTextureInfo(resourceTexture);
		break; 
	}

	case ResourceType::RESOURCE_AUDIO:

		break;
	}
}

void FileBrowserDockPanel::DrawResourceTextureInfo(Texture* resourceTexture)
{
	ImGui::Spacing();
	ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0.12f, 0.14f, 0.17f, 1.00f));
	ImGui::BeginChild("TextureData", ImVec2(ImGui::GetContentRegionMax().x - 5, 60));

	ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 5); 
	ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 5); 
	ImGui::PushFont(App->moduleImGui->rudaBlackMid);
	ImGui::Text(std::string(resourceTexture->GetName() + ":").c_str());
	ImGui::PopFont();

	ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 5);
	ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 5);
	ImGui::Text("Dimensions: "); ImGui::SameLine(); 

	ImGui::EndChild();
	ImGui::PopStyleColor();
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

ImVec2 FileBrowserDockPanel::GetImageDimensionsInPreview(Texture* texture)
{
	ImVec2 imageSize; 
	ImVec2 childSize = ImVec2(ImGui::GetContentRegionMax().x, ImGui::GetContentRegionMax().y); 
	bool isVertical = texture->IsVertical(); 
	float imageAspectRatio = texture->GetAspectRatio(); 

	if (isVertical)
	{
		imageSize.y = childSize.y; 		
		imageSize.x = imageSize.y * imageAspectRatio;
	}
	else
	{
		imageSize.x = childSize.x;
		imageSize.y = imageSize.x / imageAspectRatio;
	}

	return imageSize;
}
