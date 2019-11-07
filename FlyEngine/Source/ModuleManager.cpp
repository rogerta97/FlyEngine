#include "ModuleManager.h"

#include "ImageImporter.h"
#include "ResourceManager.h"
#include "MyFileSystem.h"

ModuleManager::ModuleManager(bool start_enabled)
{
}

ModuleManager::~ModuleManager()
{
}

bool ModuleManager::Init()
{
	// Load Engine Icons -----
	Texture* plusIcon = ImageImporter::getInstance()->LoadTexture(string(MyFileSystem::getInstance()->GetIconsDirectory() + "PlusSign.png"), false); 
	//ResourceManager::getInstance()->

	return true;
}

