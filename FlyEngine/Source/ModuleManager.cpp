#include "ModuleManager.h"

#include "ImageImporter.h"
#include "ResourceManager.h"
#include "MyFileSystem.h"
#include "Application.h"
#include "ModuleRoomManager.h"
#include "Room.h"
#include "ModuleImGui.h"
#include "ViewportManager.h"
#include "imgui.h"
#include "SceneDockPanel.h"
#include "mmgr.h"


ModuleManager::ModuleManager(bool start_enabled)
{
}

ModuleManager::~ModuleManager()
{
}

bool ModuleManager::Init()
{
	// Load Engine Icons -----
	LoadEngineIcons();

	// Tools Descriptions 
	AddToolsNameDescription("Image", "This should be the description of the image tool", AT_IMAGE);
	AddToolsNameDescription("Change Scene", "This should be the description of the change scene tool", AT_CHANGE_SCENE);

	return true;
}

bool ModuleManager::CleanUp()
{
	ImageImporter::getInstance()->Delete(); 
	ResourceManager::getInstance()->CleanUp(); 
	ViewportManager::getInstance()->Delete();
	return true;
}

void ModuleManager::LoadEngineIcons()
{
	Texture* plusIcon = ImageImporter::getInstance()->LoadTexture(string(MyFileSystem::getInstance()->GetIconsDirectory() + "PlusSign.png"), false);
	ResourceManager::getInstance()->AddResource((Resource*)plusIcon, "PlusIcon");

	Texture* minusIcon = ImageImporter::getInstance()->LoadTexture(string(MyFileSystem::getInstance()->GetIconsDirectory() + "MinusIcon.png"), false);
	ResourceManager::getInstance()->AddResource((Resource*)minusIcon, "MinusIcon");

	Texture* searchIcon = ImageImporter::getInstance()->LoadTexture(string(MyFileSystem::getInstance()->GetIconsDirectory() + "SearchIcon.png"), false);
	ResourceManager::getInstance()->AddResource((Resource*)searchIcon, "SearchIcon");

	Texture* imageNull = ImageImporter::getInstance()->LoadTexture(string(MyFileSystem::getInstance()->GetIconsDirectory() + "ImageNull.jpg"), false);
	ResourceManager::getInstance()->AddResource((Resource*)imageNull, "ImageNull");

	Texture* objectIcon = ImageImporter::getInstance()->LoadTexture(string(MyFileSystem::getInstance()->GetIconsDirectory() + "ObjectIcon.png"), false);
	ResourceManager::getInstance()->AddResource((Resource*)objectIcon, "ObjectIcon");

	Texture* filterIcon = ImageImporter::getInstance()->LoadTexture(string(MyFileSystem::getInstance()->GetIconsDirectory() + "FilterIcon.png"), false);
	ResourceManager::getInstance()->AddResource((Resource*)filterIcon, "FilterIcon");

	Texture* plusIconWhite = ImageImporter::getInstance()->LoadTexture(string(MyFileSystem::getInstance()->GetIconsDirectory() + "PlusIconWhite.png"), false);
	ResourceManager::getInstance()->AddResource((Resource*)plusIconWhite, "PlusIconWhite");

	Texture* minusIconWhite = ImageImporter::getInstance()->LoadTexture(string(MyFileSystem::getInstance()->GetIconsDirectory() + "MinusIconWhite.png"), false);
	ResourceManager::getInstance()->AddResource((Resource*)minusIconWhite, "MinusIconWhite");

	Texture* arrowUpWhite = ImageImporter::getInstance()->LoadTexture(string(MyFileSystem::getInstance()->GetIconsDirectory() + "ArrowUpWhite.png"), false);
	ResourceManager::getInstance()->AddResource((Resource*)arrowUpWhite, "ArrowUpWhite");

	Texture* arrowDownWhite = ImageImporter::getInstance()->LoadTexture(string(MyFileSystem::getInstance()->GetIconsDirectory() + "ArrowUpWhite.png"), true);
	ResourceManager::getInstance()->AddResource((Resource*)arrowDownWhite, "ArrowDownWhite");

	Texture* selectArrow = ImageImporter::getInstance()->LoadTexture(string(MyFileSystem::getInstance()->GetIconsDirectory() + "SelectArrow.png"), false);
	ResourceManager::getInstance()->AddResource((Resource*)selectArrow, "SelectArrow");

	Texture* moveOption = ImageImporter::getInstance()->LoadTexture(string(MyFileSystem::getInstance()->GetIconsDirectory() + "MoveOption.png"), true);
	ResourceManager::getInstance()->AddResource((Resource*)moveOption, "MoveOption");

	Texture* clickableAreaPreviewColor = ImageImporter::getInstance()->LoadTexture(string(MyFileSystem::getInstance()->GetIconsDirectory() + "ClickableAreaPreviewColor.png"), true);
	ResourceManager::getInstance()->AddResource((Resource*)clickableAreaPreviewColor, "ClickableAreaPreviewColor");
}

int ModuleManager::GetToolsAmount() const
{
	return toolNamesDescriptions.size();
}

ToolSelectableInfo* ModuleManager::DrawToolDictionaryUI()
{
	ToolSelectableInfo* returnInfo = nullptr; 

	for (auto& currentToolDescription : toolNamesDescriptions)
	{
		ImGui::PushFont(App->moduleImGui->rudaBoldMid);

		if (ImGui::Selectable(currentToolDescription.toolName.c_str(), false, ImGuiSelectableFlags_None, ImVec2(ImGui::GetContentRegionAvailWidth(), 37)))
		{
			returnInfo = &currentToolDescription; 
		}
		ImGui::PopFont();

		// Description -----
		ImGui::SetCursorPosY(ImGui::GetCursorPos().y - 20);
		ImGui::SetCursorPosX(ImGui::GetCursorPos().x + 2);

		ImGui::PushFont(App->moduleImGui->rudaRegularSmall);
		ImGui::TextWrapped(currentToolDescription.toolDescription.c_str());
		ImGui::PopFont();
	}

	return returnInfo; 
}

FlyObject* ModuleManager::GetSelectedFlyObject()
{
	return App->moduleRoomManager->GetSelectedRoom()->GetSelectedObject();
}

void ModuleManager::SetSelectedFlyObject(FlyObject* newSelectedObject)
{
	App->moduleRoomManager->GetSelectedRoom()->SetSelectedObject(newSelectedObject);
}

std::list<ToolSelectableInfo> ModuleManager::GetToolsNamesDescriptionsList() const
{
	return toolNamesDescriptions;
}

void ModuleManager::AddToolsNameDescription(std::string name, std::string description, ToolType newTooltype)
{
	ToolSelectableInfo newtsi; 

	newtsi.toolName = name; 
	newtsi.toolDescription = description; 
	newtsi.toolType = newTooltype; 

	toolNamesDescriptions.push_back(newtsi);
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

