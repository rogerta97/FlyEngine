#include "ResourceManager.h"
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

	newResource->SetName(name);
	instance->resourceList.push_back(newResource);
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

void ResourceManager::CleanUp()
{
	for (auto& currentResource : instance->resourceList)
	{
		currentResource->CleanUp();
		delete currentResource;
	}

	instance->resourceList.clear(); 
	delete instance; 
}