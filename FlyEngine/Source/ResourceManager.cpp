#include "ResourceManager.h"
#include "Texture.h"
#include "ImageImporter.h"
#include "MyFileSystem.h"

#include "AudioClip.h"
#include "MusicTrack.h"
#include "AudioImporter.h"
#include "imgui.h"

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
	//assert(newResource->GetPath() == ""); 

	if(newResource != nullptr)
	{
		newResource->SetName(name);
		instance->resourceList.push_back(newResource);
	}

	return false;
}

ResourceType ResourceManager::GetResourceTypeFromExtension(FileExtension ext)
{
	switch (ext)
	{
	case FILE_WAV:
		return ResourceType::RESOURCE_SFX;

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

	throw std::logic_error("Could not found resource"); 
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

void ResourceManager::PrintImagesSelectionPopup()
{
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

void ResourceManager::LoadResource(string newResourcePath, ResourceType forceType)
{
	FileExtension fileExtension = MyFileSystem::getInstance()->GetFileExtension(newResourcePath); 
	ResourceType loadingResourceType = ResourceType::RESOURCE_null;

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

	case RESOURCE_MUSIC:
	{
		MusicTrack *newResource = AudioImporter::getInstance()->LoadMusicTrack(newResourcePath);
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
			currentResource->CleanUp();
			delete currentResource;
			currentResource = nullptr; 
		}
	}

	instance->resourceList.clear(); 
	delete instance; 
}