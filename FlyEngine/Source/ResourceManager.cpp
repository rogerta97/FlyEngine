#include "ResourceManager.h"
#include <string>

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
	newResource->SetName(name);
	resourceList.push_back(newResource);
	return false;
}

Resource* ResourceManager::GetResource(std::string resourceName) const
{
	for (auto& it : resourceList)
		if ((it)->GetName() == resourceName)
			return (it);

	FLY_ERROR("No resource %s found", resourceName.c_str());
	return nullptr;
}

Resource* ResourceManager::GetResource(UID resourceUID) const
{
	for (auto& it : resourceList)
		if ((it)->GetUID() == resourceUID)
			return (it);

	FLY_ERROR("No resource with UID %d found", resourceUID);
	return nullptr;
}

bool ResourceManager::ExistResource(Resource* checkResource)
{
	return ExistResource(checkResource->GetUID());
}

bool ResourceManager::ExistResource(UID resourceUID)
{
	for (auto& it: resourceList)
	{
		if (it->GetUID() == resourceUID)
			return true;
	}

	return false; 
}

void ResourceManager::Delete()
{
	delete instance;
}