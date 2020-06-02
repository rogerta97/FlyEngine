#include "FileBrowserDockPanel.h"
#include "MyFileSystem.h"
#include "ResourceManager.h"
#include "ModuleImGui.h"
#include "GameViewportDockPanel.h"
#include "Texture.h"
#include "AudioClip.h"
#include "MusicTrack.h"
#include "imgui.h"

#include "mmgr.h"

FileBrowserDockPanel::FileBrowserDockPanel(bool isVisible) : DockPanel("Resources", isVisible)
{
	flyEngineSection = FLY_SECTION_null;
	dockPanelType = DOCK_FILE_BROWSER;
	selectedResourceUID = 0; 
}

FileBrowserDockPanel::~FileBrowserDockPanel()
{

}

bool FileBrowserDockPanel::Draw()
{
	if (!DockPanel::Draw())
		return false;

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
	if (selectedResourceUID == 0) return; 
	
	Resource* selectedResource = ResourceManager::getInstance()->GetResource(selectedResourceUID);

	switch (selectedResource->GetType())
	{

	case ResourceType::RESOURCE_TEXTURE:
	{
		DrawTextureResourcePreview(selectedResource);
		break; 
	}

	case ResourceType::RESOURCE_SFX:
	{
		DrawAudioClipResourcePreview(selectedResource);
		break;
	}

	case ResourceType::RESOURCE_MUSIC:
	{
		DrawMusicTrackResourcePreview(selectedResource);
		break;
	}
	case ResourceType::RESOURCE_FONT:
	{
		DrawMusicTrackResourcePreview(selectedResource);
		break;
	}
	}
}

void FileBrowserDockPanel::DrawMusicTrackResourcePreview(Resource* selectedResource)
{
	MusicTrack* selectedMusicTrack = (MusicTrack*)selectedResource;
	FileExtension ext = MyFileSystem::getInstance()->GetFileExtension(selectedResource->GetPath());
	Texture* fileTypeTexture = nullptr;

	switch (ext)
	{
	case FILE_MP3:
		fileTypeTexture = (Texture*)ResourceManager::getInstance()->GetResource("MP3Icon");
		break;
	case FILE_WAV:
		fileTypeTexture = (Texture*)ResourceManager::getInstance()->GetResource("WAVIcon");
		break;
	}

	if (fileTypeTexture != nullptr)
	{
		ImVec2 drawPoint = ImVec2((ImGui::GetContentRegionMax().x / 2) - 10, 80);
		float imageMaxWidth = 130;
		ImVec2 imageDimensions = ImVec2(imageMaxWidth, imageMaxWidth * fileTypeTexture->GetAspectRatio());

		ImGui::SetCursorPos(ImVec2(ImGui::GetCursorPosX() + drawPoint.x - ((imageDimensions.x) / 2), ImGui::GetCursorPosY() + drawPoint.y - ((imageDimensions.y) / 2)));
		ImGui::Image((ImTextureID)fileTypeTexture->GetTextureID(), ImVec2(imageDimensions.x, imageDimensions.y));
	}

	ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0.12f, 0.14f, 0.17f, 1.00f));
	ImGui::BeginChild("BrowserPreview", ImVec2(210, 95));

	ImGui::SetCursorPos(ImVec2(ImGui::GetCursorPos().x + 8, ImGui::GetCursorPos().y + 8));
	ImGui::PushFont(App->moduleImGui->rudaBoldBig);
	ImGui::Text(selectedMusicTrack->GetName().c_str());
	ImGui::PopFont();

	ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 5);
	Texture* playAudioTexture = (Texture*)ResourceManager::getInstance()->GetResource("PlayAudio");
	if (ImGui::ImageButton((ImTextureID)playAudioTexture->GetTextureID(), ImVec2(25, 25)))
	{
		selectedMusicTrack->Play(); 
	}
	ImGui::SameLine();

	Texture* stopAudioTexture = (Texture*)ResourceManager::getInstance()->GetResource("PauseAudio");
	if (ImGui::ImageButton((ImTextureID)stopAudioTexture->GetTextureID(), ImVec2(25, 25)))
	{
		selectedMusicTrack->Stop(); 
	}

	ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 5);
	ImGui::PushFont(App->moduleImGui->rudaRegularSmall);
	ImGui::Text("Audio Duration: ");
	ImGui::PopFont();

	ImGui::EndChild();
	ImGui::PopStyleColor();
}

void FileBrowserDockPanel::DrawTextureResourcePreview(Resource* selectedResource)
{
	ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0.05f, 0.05f, 0.05f, 1.00f));
	ImGui::BeginChild("BrowserPreview", ImVec2(210, 240), true);

	Texture* resourceTexture = (Texture*)selectedResource;
	ImVec2 centerPoint = ImVec2(ImGui::GetContentRegionMax().x / 2, ImGui::GetContentRegionMax().y / 2);
	ImVec2 imageProportions = GetImageDimensionsInPreview(resourceTexture);

	ImGui::SetCursorPos(ImVec2(ImGui::GetCursorPosX() + centerPoint.x - ((imageProportions.x) / 2), ImGui::GetCursorPosY() + centerPoint.y - ((imageProportions.y) / 2)));
	ImGui::Image((ImTextureID)resourceTexture->GetTextureID(), ImVec2(imageProportions.x - 8, imageProportions.y - 10));

	ImGui::EndChild();
	ImGui::PopStyleColor();

	DrawResourceTextureInfo(resourceTexture);
}

void FileBrowserDockPanel::DrawAudioClipResourcePreview(Resource* selectedResource)
{
	AudioClip* selectedAudioClip = (AudioClip*)selectedResource;
	FileExtension ext = MyFileSystem::getInstance()->GetFileExtension(selectedResource->GetPath());
	Texture* fileTypeTexture = nullptr;

	switch (ext)
	{
	case FILE_MP3:
		fileTypeTexture = (Texture*)ResourceManager::getInstance()->GetResource("MP3Icon");
		break;
	case FILE_WAV:
		fileTypeTexture = (Texture*)ResourceManager::getInstance()->GetResource("WAVIcon");
		break;
	}

	if (fileTypeTexture != nullptr)
	{
		ImVec2 drawPoint = ImVec2((ImGui::GetContentRegionMax().x / 2) - 10, 80);
		float imageMaxWidth = 130;
		ImVec2 imageDimensions = ImVec2(imageMaxWidth, imageMaxWidth * fileTypeTexture->GetAspectRatio());

		ImGui::SetCursorPos(ImVec2(ImGui::GetCursorPosX() + drawPoint.x - ((imageDimensions.x) / 2), ImGui::GetCursorPosY() + drawPoint.y - ((imageDimensions.y) / 2)));
		ImGui::Image((ImTextureID)fileTypeTexture->GetTextureID(), ImVec2(imageDimensions.x, imageDimensions.y));
	}

	ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0.12f, 0.14f, 0.17f, 1.00f));
	ImGui::BeginChild("BrowserPreview", ImVec2(210, 95));

	ImGui::SetCursorPos(ImVec2(ImGui::GetCursorPos().x + 8, ImGui::GetCursorPos().y + 8));
	ImGui::PushFont(App->moduleImGui->rudaBoldBig);
	ImGui::Text(string(selectedAudioClip->GetName() + ":").c_str());
	ImGui::PopFont();

	ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 5);
	Texture* playAudioTexture = (Texture*)ResourceManager::getInstance()->GetResource("PlayAudio");
	if (ImGui::ImageButton((ImTextureID)playAudioTexture->GetTextureID(), ImVec2(25, 25)))
	{
		selectedAudioClip->Play(); 
	}

	ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 5);
	ImGui::PushFont(App->moduleImGui->rudaRegularSmall);
	ImGui::Text("Audio Duration: ");
	ImGui::PopFont();

	ImGui::EndChild();
	ImGui::PopStyleColor();
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

	if (fileName == "EngineResources")
		return; 

	if (MyFileSystem::getInstance()->IsFolder(directory))
	{
		Texture* folderTexture = (Texture*)ResourceManager::getInstance()->GetResource("FolderIcon"); 

		ImGui::Image((ImTextureID)folderTexture->GetTextureID(), ImVec2(20, 20)); ImGui::SameLine();
		ImGui::SetCursorPosX(ImGui::GetCursorPosX() - 5);
		if(ImGui::TreeNodeEx(fileName.c_str(), ImGuiTreeNodeFlags_DefaultOpen))
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
		resourceName = MyFileSystem::getInstance()->DeleteFileExtension(resourceName);
		Resource* currentResource = ResourceManager::getInstance()->GetResource(resourceName.c_str());

		ImTextureID iconTextureID = 0;

		FileExtension fileExtension = MyFileSystem::getInstance()->GetFileExtension(fileName);
		switch (fileExtension)
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
		case FILE_MP3:
		{
			Texture* iconTexture = nullptr; 

			if(currentResource->GetType() == ResourceType::RESOURCE_MUSIC)
				iconTexture = (Texture*)ResourceManager::getInstance()->GetResource("MusicIcon");
			else 
				iconTexture = (Texture*)ResourceManager::getInstance()->GetResource("SpeakerIcon");

			iconTextureID = (ImTextureID)iconTexture->GetTextureID();
			break;
		}
		case FILE_WAV:
		{
			Texture* iconTexture = nullptr;

			if (currentResource->GetType() == ResourceType::RESOURCE_MUSIC)
				iconTexture = (Texture*)ResourceManager::getInstance()->GetResource("MusicIcon");
			else
				iconTexture = (Texture*)ResourceManager::getInstance()->GetResource("SpeakerIcon");

			iconTextureID = (ImTextureID)iconTexture->GetTextureID();
			break;
		}
		case FILE_TTF:
		{
			Texture* jpgTexture = (Texture*)ResourceManager::getInstance()->GetResource("FontIcon");
			iconTextureID = (ImTextureID)jpgTexture->GetTextureID();
			break;
		}
		case FILE_OTF:
		{
			Texture* jpgTexture = (Texture*)ResourceManager::getInstance()->GetResource("FontIcon");
			iconTextureID = (ImTextureID)jpgTexture->GetTextureID();
			break;
		}
		}
		
		ImGui::Image(iconTextureID, ImVec2(20, 20)); ImGui::SameLine();

		bool selected = false;
		if (currentResource != nullptr)
		{
			if (selectedResourceUID == currentResource->GetUID())
				selected = true; 
		}

		if (ImGui::Selectable(fileName.c_str(), &selected))
		{
			fileName = MyFileSystem::getInstance()->DeleteFileExtension(fileName);

			if(currentResource != nullptr)
				selectedResourceUID = currentResource->GetUID();
		}

		if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_None))
		{
			int resTypeIntCast = (int)currentResource->GetUID(); 
			ImGui::SetDragDropPayload("drag_resource", &resTypeIntCast, 256);

			ImGui::Image(iconTextureID, ImVec2(35, 35)); 
			ImGui::SameLine();

			std::string resourceName = currentResource->GetName(); 

			ImGui::SetCursorPosY(16);
			ImGui::PushFont(App->moduleImGui->rudaBlackBig);
			ImGui::Text(resourceName.c_str());
			ImGui::PopFont(); 

			ImGui::EndDragDropSource();
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
