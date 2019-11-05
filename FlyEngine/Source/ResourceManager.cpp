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

void ResourceManager::Delete()
{
	delete instance;
}