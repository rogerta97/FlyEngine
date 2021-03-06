#include "ResourceManager.h"
#include "Texture.h"
#include "ImageImporter.h"
#include "MyFileSystem.h"

#include "AudioClip.h"
#include "MusicTrack.h"
#include "AudioImporter.h"
#include "FontImporter.h"
#include "imgui.h"
#include "Font.h"

#include <string>
#include "mmgr.h"

#include <assert.h>

ResourceManager* ResourceManager::instance = 0;

ResourceManager::ResourceManager()
{

}

ResourceManager* ResourceManager::getInstance()
{
	if (instance == nullptr)
		instance = new ResourceManager();

	return instance;
}

ResourceManager::~ResourceManager()
{
}

bool ResourceManager::AddResource(Resource* newResource, std::string name)
{
	if(newResource != nullptr)
	{
		newResource->SetName(name);
		instance->resourceList.push_back(newResource);
		return true; 
	}

	return false;
}

ResourceType ResourceManager::GetResourceTypeFromExtension(FileExtension ext)
{
	switch (ext)
	{
	case FILE_WAV:
		return ResourceType::RESOURCE_SFX;

	case FILE_OTF:
		return ResourceType::RESOURCE_FONT;

	case FILE_TTF:
		return ResourceType::RESOURCE_FONT;

	case FILE_MP3:
		return ResourceType::RESOURCE_MUSIC;

	case FILE_PNG:
		return ResourceType::RESOURCE_TEXTURE;

	case FILE_JPG:
		return ResourceType::RESOURCE_TEXTURE;
	}

	return ResourceType::RESOURCE_null; 
}

Resource* ResourceManager::GetResource(std::string _resourceName, ResourceType type) 
{
	for (auto& it : instance->resourceList)
	{
		string resourceName = MyFileSystem::getInstance()->DeleteFileExtension((it)->GetName()); 
		if (resourceName == _resourceName)
		{
			if (type != RESOURCE_null)
			{
				if ((it)->GetType() == type)
					return (it);
				else
					continue; 
			}

			return (it);
		}
	}

	return nullptr;
}

Resource* ResourceManager::GetResource(UID resourceUID, ResourceType type)
{
	for (auto& it : instance->resourceList)
	{
		if ((it)->GetUID() == resourceUID)
		{
			if (type == RESOURCE_null)
				return (it);
			else
			{
				if ((it)->GetType() == type)
					return (it);
				else
					continue;
			}
		}
	}

	FLY_ERROR("No resource with UID %d found", resourceUID);
	return nullptr;
}

vector<Resource*> ResourceManager::GetResources(ResourceType type)
{
	vector<Resource*> retList = vector<Resource*>();
	for (auto& currentResource : instance->resourceList)
	{
		if (currentResource->GetType() == type)
			retList.push_back(currentResource); 
	}

	return retList;
}

list<Resource*>& ResourceManager::GetResourceList()
{
	return instance->resourceList; 
}

Resource* ResourceManager::GetResourceByPath(std::string resourcePath) 
{
	for (auto& it : instance->resourceList)
		if ((it)->GetPath() == resourcePath)
			return (it);

	FLY_ERROR("No resource with path '%s' found", resourcePath.c_str());
	return nullptr;
}

Texture* ResourceManager::GetTexture(string resourceName)
{
	return (Texture*)GetResource(resourceName, RESOURCE_TEXTURE);
}

Font* ResourceManager::GetFont(string resourceName)
{
	return (Font*)GetResource(resourceName, RESOURCE_FONT);
}

AudioClip* ResourceManager::GetAudioClip(string resourceName)
{
	return (AudioClip*)GetResource(resourceName, RESOURCE_SFX);
}

MusicTrack* ResourceManager::GetMusicTrack(string resourceName)
{
	return (MusicTrack*)GetResource(resourceName, RESOURCE_MUSIC);
}

bool ResourceManager::ExistResource(Resource* checkResource)
{
	return ExistResourceUID(checkResource->GetUID());
}

bool ResourceManager::ExistResourceUID(UID resourceUID)
{
	for (auto& it: instance->resourceList)
	{
		if (it->GetUID() == resourceUID)
			return true;
	}

	return false; 
}

bool ResourceManager::ExistResourcePath(std::string resourcePath)
{
	for (auto& it : instance->resourceList)
	{
		if (it->GetPath() == resourcePath)
			return true;
	}

	return false;
}

Resource* ResourceManager::PrintImagesSelectionPopup()
{
	if (ImGui::BeginPopup("print_image_selection_popup"))
	{
		ImGui::Spacing();

		// Search Bar ---------------
		static char searchImageBuffer[256];
		ImGui::InputTextWithHint("##SearchTool", "Search...", searchImageBuffer, IM_ARRAYSIZE(searchImageBuffer));
		ImGui::SameLine();

		Texture* filterIcon = (Texture*)ResourceManager::getInstance()->GetResource("FilterIcon");
		ImGui::Image((ImTextureID)filterIcon->GetTextureID(), ImVec2(22, 22));

		ImGui::Spacing();

		ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0.12f, 0.14f, 0.17f, 1.00f));
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(2.0f, 2.0f));
		ImGui::BeginChild("##4ShowImage", ImVec2(ImGui::GetContentRegionAvailWidth(), 150));

		for (auto& currentResource : instance->resourceList)
		{
			if (currentResource->GetType() != ResourceType::RESOURCE_TEXTURE)
				continue;

			Texture* imageIcon = (Texture*)currentResource;

			ImGui::Image((ImTextureID)imageIcon->GetTextureID(), ImVec2(30, 30));
			ImGui::SameLine();

			if (ImGui::Selectable(currentResource->GetName().c_str(), false, 0, ImVec2(ImGui::GetContentRegionAvail().x, 25)))
			{
				ImGui::EndChild();
				ImGui::PopStyleColor();
				ImGui::PopStyleVar();
				ImGui::CloseCurrentPopup();
				ImGui::EndPopup();
				return currentResource;
			}
		}

		ImGui::EndChild();
		ImGui::PopStyleColor();
		ImGui::PopStyleVar();
		ImGui::EndPopup();
	}

	return nullptr; 
}

Resource* ResourceManager::PrintSoundsSelectionPopup()
{
	if (ImGui::BeginPopup("print_sound_selection_popup"))
	{
		static char searchSoundBuffer[256];
		ImGui::InputTextWithHint("##SearchSound", "Search...", searchSoundBuffer, IM_ARRAYSIZE(searchSoundBuffer));
		ImGui::Separator();

		for (auto& currentResource : instance->resourceList)
		{
			if (currentResource->GetType() != ResourceType::RESOURCE_SFX)
				continue; 

			Texture* speakerIcon = (Texture*)ResourceManager::getInstance()->GetResource("SpeakerIcon");

			ImGui::Image((ImTextureID)speakerIcon->GetTextureID(), ImVec2(20, 20));
			ImGui::SameLine();

			if (ImGui::Selectable(currentResource->GetName().c_str()))
			{
				ImGui::EndPopup();
				return currentResource;
			}
		}

		ImGui::EndPopup(); 
	}

	return nullptr; 
}

Resource* ResourceManager::PrintSoundsSelectionPopup2()
{
	if (ImGui::BeginPopup("print_sound_selection_popup2"))
	{
		static char searchSoundBuffer[256];
		ImGui::InputTextWithHint("##SearchSound", "Search...", searchSoundBuffer, IM_ARRAYSIZE(searchSoundBuffer));
		ImGui::Separator();

		for (auto& currentResource : instance->resourceList)
		{
			if (currentResource->GetType() != ResourceType::RESOURCE_SFX)
				continue;

			Texture* speakerIcon = (Texture*)ResourceManager::getInstance()->GetResource("SpeakerIcon");

			ImGui::Image((ImTextureID)speakerIcon->GetTextureID(), ImVec2(20, 20));
			ImGui::SameLine();

			if (ImGui::Selectable(currentResource->GetName().c_str()))
			{
				ImGui::EndPopup();
				return currentResource;
			}
		}

		ImGui::EndPopup();
	}

	return nullptr;
}

Resource* ResourceManager::PrintSoundsSelectionPopup3()
{
	if (ImGui::BeginPopup("print_sound_selection_popup3"))
	{
		static char searchSoundBuffer[256];
		ImGui::InputTextWithHint("##SearchSound", "Search...", searchSoundBuffer, IM_ARRAYSIZE(searchSoundBuffer));
		ImGui::Separator();

		for (auto& currentResource : instance->resourceList)
		{
			if (currentResource->GetType() != ResourceType::RESOURCE_SFX)
				continue;

			Texture* speakerIcon = (Texture*)ResourceManager::getInstance()->GetResource("SpeakerIcon");

			ImGui::Image((ImTextureID)speakerIcon->GetTextureID(), ImVec2(20, 20));
			ImGui::SameLine();

			if (ImGui::Selectable(currentResource->GetName().c_str()))
			{
				ImGui::EndPopup();
				return currentResource;
			}
		}

		ImGui::EndPopup();
	}

	return nullptr;
}

Resource* ResourceManager::PrintMusicSelectionPopup()
{
	if (ImGui::BeginPopup("print_music_selection_popup"))
	{
		static char searchSoundBuffer[256];
		ImGui::InputTextWithHint("##SearchMusic", "Search...", searchSoundBuffer, IM_ARRAYSIZE(searchSoundBuffer));
		ImGui::Separator();

		for (auto& currentResource : instance->resourceList)
		{
			if (currentResource->GetType() != ResourceType::RESOURCE_MUSIC)
				continue;

			Texture* speakerIcon = (Texture*)ResourceManager::getInstance()->GetResource("SpeakerIcon");

			ImGui::Image((ImTextureID)speakerIcon->GetTextureID(), ImVec2(20, 20));
			ImGui::SameLine();

			if (ImGui::Selectable(currentResource->GetName().c_str()))
			{
				ImGui::EndPopup();
				return currentResource;
			}
		}

		ImGui::EndPopup();
	}

	return nullptr;
}

Resource* ResourceManager::PrintFontSelectionPopup()
{
	if (ImGui::BeginPopup("print_font_selection_popup"))
	{
		static char searchFontBuffer[256];
		ImGui::InputTextWithHint("##SearchFont", "Search...", searchFontBuffer, IM_ARRAYSIZE(searchFontBuffer));
		ImGui::Separator();

		for (auto& currentResource : instance->resourceList)
		{
			if (currentResource->GetType() != ResourceType::RESOURCE_FONT)
				continue;

			Texture* speakerIcon = (Texture*)ResourceManager::getInstance()->GetResource("DisplayTextIcon");

			ImGui::Image((ImTextureID)speakerIcon->GetTextureID(), ImVec2(20, 20));
			ImGui::SameLine();

			if (ImGui::Selectable(currentResource->GetName().c_str()))
			{
				ImGui::EndPopup();
				return currentResource;
			}
		}

		ImGui::EndPopup();
	}

	return nullptr;
}

string ResourceManager::PrintFolderSelectionPopup(string parentFolder)
{
	if (ImGui::BeginPopup("print_folder_selection_popup"))
	{
		ImGui::Spacing();

		// Search Bar ---------------
		static char searchImageBuffer[256];
		ImGui::InputTextWithHint("##SearchTool", "Search...", searchImageBuffer, IM_ARRAYSIZE(searchImageBuffer));
		ImGui::SameLine();

		Texture* filterIcon = (Texture*)ResourceManager::getInstance()->GetResource("FilterIcon");
		ImGui::Image((ImTextureID)filterIcon->GetTextureID(), ImVec2(22, 22), ImVec2(1,1), ImVec2(0, 0));

		ImGui::Spacing();

		ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0.12f, 0.14f, 0.17f, 1.00f));
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(2.0f, 2.0f));
		ImGui::BeginChild("##4ShowImage", ImVec2(ImGui::GetContentRegionAvailWidth(), 150));

		vector<string> foldersInDirectory = MyFileSystem::getInstance()->GetFoldersInDirectory(parentFolder.c_str());
		
		for (auto& currentFolder : foldersInDirectory)
		{
			Texture* imageIcon = (Texture*)ResourceManager::getInstance()->GetResource("FolderIcon");

			ImGui::Image((ImTextureID)imageIcon->GetTextureID(), ImVec2(30, 30), ImVec2(0, 1), ImVec2(1, 0));
			ImGui::SameLine();

			string folderName = MyFileSystem::getInstance()->GetLastPathItem(currentFolder, false).c_str();
			if (ImGui::Selectable(folderName.c_str(), false, 0, ImVec2(ImGui::GetContentRegionAvail().x, 25)))
			{
				ImGui::EndChild();
				ImGui::PopStyleColor();
				ImGui::PopStyleVar();
				ImGui::CloseCurrentPopup();
				ImGui::EndPopup();
				return currentFolder;
			}
		}

		ImGui::EndChild();
		ImGui::PopStyleColor();
		ImGui::PopStyleVar();
		ImGui::EndPopup();
	}

	return ""; 
}


void ResourceManager::LoadResource(string newResourcePath, ResourceType forceType)
{
	FileExtension fileExtension = MyFileSystem::getInstance()->GetFileExtension(newResourcePath); 
	ResourceType loadingResourceType = ResourceType::RESOURCE_null;

	if (fileExtension == FILE_null)
	{
		FLY_ERROR("Loading unrecognized file %d", newResourcePath.c_str());
		return;
	}

	bool forced = false; 
	if (forceType != RESOURCE_null)
	{
		loadingResourceType = forceType; 
		forced = true; 
	}

	if (!forced)
		loadingResourceType = GetResourceTypeFromExtension(fileExtension); 
	
	switch (loadingResourceType)
	{
	case RESOURCE_TEXTURE:
	{
		Texture* newResource = ImageImporter::getInstance()->LoadTexture(newResourcePath, false);
		string resourceName = MyFileSystem::getInstance()->GetLastPathItem(newResourcePath, false);

		//if (resourceName == "Door")
		//	flog("hids"); 

		AddResource(newResource, resourceName.c_str());
		flog("Added Resource %s", resourceName.c_str());
	}
		break; 

	case RESOURCE_SFX:
	{
		AudioClip* newResource = AudioImporter::getInstance()->LoadAudioClip(newResourcePath);
		string resourceName = MyFileSystem::getInstance()->GetLastPathItem(newResourcePath, false);
		AddResource(newResource, resourceName.c_str());
		flog("Added Resource %s", resourceName.c_str());
	}
		break; 

	case RESOURCE_MUSIC:
	{
		MusicTrack *newResource = AudioImporter::getInstance()->LoadMusicTrack(newResourcePath);
		string resourceName = MyFileSystem::getInstance()->GetLastPathItem(newResourcePath, false);
		AddResource(newResource, resourceName.c_str());
		flog("Added Resource %s", resourceName.c_str());
	}
		break;

	case RESOURCE_FONT:
	{
		Font* newResource = FontImporter::getInstance()->LoadFont(newResourcePath);
		string resourceName = MyFileSystem::getInstance()->GetLastPathItem(newResourcePath, false);
		AddResource(newResource, resourceName.c_str());
		flog("Added Resource %s", resourceName.c_str());
	}
		break;
	}
	
}

void ResourceManager::LoadAllGameResources()
{
	std::string resourcesImagePath = MyFileSystem::getInstance()->GetResourcesDirectory() + "\\Images"; 
	LoadAllFilesFromFolder(resourcesImagePath);

	std::string resourcesAudioPath = MyFileSystem::getInstance()->GetResourcesDirectory() + "\\Audio\\Effects";
	LoadAllFilesFromFolder(resourcesAudioPath, RESOURCE_SFX);

	std::string resourcesMusicPath = MyFileSystem::getInstance()->GetResourcesDirectory() + "\\Audio\\Music";
	LoadAllFilesFromFolder(resourcesMusicPath, RESOURCE_MUSIC);

	std::string resourcesFontsPath = MyFileSystem::getInstance()->GetResourcesDirectory() + "\\Fonts";
	LoadAllFilesFromFolder(resourcesFontsPath, RESOURCE_FONT);

	std::string tumbnailsPath = MyFileSystem::getInstance()->GetThumbnilesDirectory();
	LoadAllFilesFromFolder(tumbnailsPath, RESOURCE_TEXTURE);

	string animationsPath = MyFileSystem::getInstance()->GetResourcesDirectory() + "\\Animations"; 
	std::vector<string> folders = MyFileSystem::getInstance()->GetFoldersInDirectory(animationsPath.c_str());

	for (auto& currentFolder : folders)
	{
		LoadAllFilesFromFolder(currentFolder);
	}
}

void ResourceManager::LoadAllFilesFromFolder(string path, ResourceType forceType)
{
	if (MyFileSystem::getInstance()->IsFolder(path))
	{
		vector<string> filesInPath;
		MyFileSystem::getInstance()->GetFilesInDirectory(path.c_str(), filesInPath, true);

		for (auto& currentFile : filesInPath)
		{
			std::string fileName = MyFileSystem::getInstance()->GetLastPathItem(currentFile, true);
			std::string currentPath = path + "\\" + fileName.c_str();

			if (MyFileSystem::getInstance()->IsFolder(currentPath))
			{
				LoadAllFilesFromFolder(currentPath);
			}
			else
			{
				LoadResource(currentPath, forceType);
			}
		}

	}
	else
	{
		LoadResource(path, forceType);
	}
}

void ResourceManager::LoadEngineIconsResources()
{
	std::vector<std::string> iconsFileName;
	MyFileSystem::getInstance()->GetFilesInDirectory(MyFileSystem::getInstance()->GetIconsDirectory().c_str(), iconsFileName);

	for (auto& currentFileName : iconsFileName)
	{
		string texturePath = string(MyFileSystem::getInstance()->GetIconsDirectory() + currentFileName);
		string resourceName = MyFileSystem::getInstance()->GetLastPathItem(texturePath, false);

		Texture* currentTexture = ImageImporter::getInstance()->LoadTexture(texturePath.c_str(), false);
		ResourceManager::getInstance()->AddResource((Resource*)currentTexture, resourceName.c_str());
	}
}

void ResourceManager::CleanUp()
{
	for (auto& currentResource : instance->resourceList)
	{
		if (currentResource != nullptr && currentResource->GetType() != RESOURCE_null)
		{
			if (currentResource->GetType() == RESOURCE_FONT)
				int a = 0; 

			currentResource->CleanUp();
			delete currentResource;
			currentResource = nullptr; 
		}
	}

	instance->resourceList.clear(); 
	delete instance; 
}