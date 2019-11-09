#include "ModuleManager.h"

#include "ImageImporter.h"
#include "ResourceManager.h"
#include "MyFileSystem.h"
#include "SceneDockPanel.h"



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
	ResourceManager::getInstance()->AddResource((Resource*)plusIcon, "PlusIcon");

	Texture* minusIcon = ImageImporter::getInstance()->LoadTexture(string(MyFileSystem::getInstance()->GetIconsDirectory() + "MinusIcon.png"), false);
	ResourceManager::getInstance()->AddResource((Resource*)minusIcon, "MinusIcon");

	Texture* searchIcon = ImageImporter::getInstance()->LoadTexture(string(MyFileSystem::getInstance()->GetIconsDirectory() + "SearchIcon.png"), false);
	ResourceManager::getInstance()->AddResource((Resource*)searchIcon, "SearchIcon");

	// Tools Descriptions 
	ToolSelectableInfo imageToolInfo; 
	imageToolInfo.toolName = "Image"; 
	imageToolInfo.toolDescription = "This should be the description of the image tool"; 
	imageToolInfo.toolType = AT_IMAGE; 

	AddToolsNameDescription(imageToolInfo); 

	return true;
}

int ModuleManager::GetToolsAmount() const
{
	return toolNamesDescriptions.size();
}

std::list<ToolSelectableInfo> ModuleManager::GetToolsNamesDescriptionsList() const
{
	return toolNamesDescriptions;
}

void ModuleManager::AddToolsNameDescription(ToolSelectableInfo newInfo)
{
	toolNamesDescriptions.push_back(newInfo); 
}

ToolSelectableInfo ModuleManager::GetToolNameDescription(std::string toolCheckName) const
{
	for (auto& it : toolNamesDescriptions)
	{
		if ((it).toolName == toolCheckName)
			return (it);
	}
}

ToolSelectableInfo ModuleManager::GetToolNameDescription(int pos) const
{
	int count = 0;
	for (auto& it : toolNamesDescriptions)
	{	
		if (count == pos)
			return (it); 

		count++;
	}

	return ToolSelectableInfo();
}

