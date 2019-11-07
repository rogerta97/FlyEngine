#include "ModuleManager.h"

#include "ImageImporter.h"
#include "ResourceManager.h"

ModuleManager::ModuleManager(bool start_enabled)
{
}

ModuleManager::~ModuleManager()
{
}

bool ModuleManager::Init()
{
	// Load Engine Icons -----
	//Texture* plusIcon = ImageImporter::getInstance()->LoadTexture(FileSystem::getInstance()->solutionDirectory + ""); 
	//ResourceManager::getInstance()->

	return true;
}

