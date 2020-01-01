#include "ResourceManager.h"
#include "MyFileSystem.h"
#include "Texture.h"
#include "ImageImporter.h"

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

Resource* ResourceManager::GetResource(std::string resourceName) 
{
	for (auto& it : instance->resourceList)
		if ((it)->GetName() == resourceName)
			return (it);

	FLY_ERROR("No resource %s found", resourceName.c_str());
	return nullptr;
}

Resource* ResourceManager::GetResource(UID resourceUID) 
{
	for (auto& it : instance->resourceList)
		if ((it)->GetUID() == resourceUID)
			return (it);

	FLY_ERROR("No resource with UID %d found", resourceUID);
	return nullptr;
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

void ResourceManager::LoadResource(string newResourcePath)
{
	FileExtension fileExtension = MyFileSystem::getInstance()->GetFileExtension(newResourcePath); 

	switch (fileExtension)
	{
	case FILE_JSON:
		break;

	case FILE_WAV:
		break;

	case FILE_PNG:
	{
		Texture* newResource = ImageImporter::getInstance()->LoadTexture(newResourcePath, false);
		string resourceName = MyFileSystem::getInstance()->GetLastPathItem(newResourcePath, false);
		AddResource(newResource, resourceName.c_str());
		flog("Added Resource %s", resourceName.c_str());
		break;
	}

	case FILE_JPG:
	{
		Texture* newResource = ImageImporter::getInstance()->LoadTexture(newResourcePath, false);
		string resourceName = MyFileSystem::getInstance()->GetLastPathItem(newResourcePath, false);
		AddResource(newResource, resourceName.c_str());
		flog("Added Resource %s", resourceName.c_str());
		break;
	}
		
	default:
		FLY_ERROR("INVALID RESOURCE WITH PATH: %s", newResourcePath.c_str()); 
		break;
	}
}

void ResourceManager::LoadAllGameResources()
{
	std::string resourcesImagePath = MyFileSystem::getInstance()->GetResourcesDirectory() + "\\Images"; 
	LoadAllFilesFromFolder(resourcesImagePath);
}

void ResourceManager::LoadAllFilesFromFolder(string path)
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
				LoadResource(currentPath);
			}
		}

	}
	else
	{
		LoadResource(path);
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