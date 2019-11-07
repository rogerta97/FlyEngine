#include "ResourceManager.h"

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

bool ResourceManager::AddResource(Resource* newResource)
{


	resourceList.push_back(newResource);
	return false;
}

bool ResourceManager::ExistResource(Resource* checkResource)
{
	return false;
}

bool ResourceManager::ExistResource(UID resourceUID)
{
	for (auto& it: resourceList)
	{
		//if(it.)
	}

	return false; 
}

void ResourceManager::Delete()
{
	delete instance;
}