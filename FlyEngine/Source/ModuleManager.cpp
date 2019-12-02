#include "ModuleManager.h"

#include "ImageImporter.h"
#include "ResourceManager.h"
#include "MyFileSystem.h"
#include "Application.h"
#include "ModuleRoomManager.h"
#include "Room.h"
#include "ModuleImGui.h"
#include "Texture.h"
#include "ViewportManager.h"
#include "imgui.h"
#include "RoomDockPanel.h"
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

	Texture* imageNull = ImageImporter::getInstance()->LoadTexture(string(MyFileSystem::getInstance()->GetIconsDirectory() + "ImageNull.png"), false);
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

	Texture* imageIcon = ImageImporter::getInstance()->LoadTexture(string(MyFileSystem::getInstance()->GetIconsDirectory() + "ImageIcon.png"), true);
	ResourceManager::getInstance()->AddResource((Resource*)imageIcon, "ImageIcon");

	Texture* background_1_1 = ImageImporter::getInstance()->LoadTexture(string(MyFileSystem::getInstance()->GetIconsDirectory() + "Background_1_1.png"), true);
	ResourceManager::getInstance()->AddResource((Resource*)background_1_1, "Background_1_1");

	Texture* playIcon = ImageImporter::getInstance()->LoadTexture(string(MyFileSystem::getInstance()->GetIconsDirectory() + "play.png"), true);
	ResourceManager::getInstance()->AddResource((Resource*)playIcon, "PlayIcon");

	Texture* saveIcon = ImageImporter::getInstance()->LoadTexture(string(MyFileSystem::getInstance()->GetIconsDirectory() + "SaveIcon.png"), false);
	ResourceManager::getInstance()->AddResource((Resource*)saveIcon, "SaveIcon");

	Texture* stopIcon = ImageImporter::getInstance()->LoadTexture(string(MyFileSystem::getInstance()->GetIconsDirectory() + "StopIcon.png"), false);
	ResourceManager::getInstance()->AddResource((Resource*)stopIcon, "StopIcon");
}

int ModuleManager::GetToolsAmount() const
{
	return toolNamesDescriptions.size();
}

ToolSelectableInfo* ModuleManager::DrawToolDictionaryUI()
{
	ToolSelectableInfo* returnInfo = nullptr; 

	int count = 0; 
	int selectableHeight = 42;

	for (auto& currentToolDescription : toolNamesDescriptions)
	{
		ImGui::PushFont(App->moduleImGui->rudaBoldMid);

		Texture* iconTexture = GetIconFromToolType(currentToolDescription.toolType); 
		ImGui::SetCursorPos(ImVec2(10, 5 + (selectableHeight * count)));
		if (iconTexture) {
			ImGui::Image((ImTextureID)iconTexture->GetTextureID(), ImVec2(30, 30), ImVec2(0, 1), ImVec2(1, 0));
		}
		else
		{
			ImGui::Image(0, ImVec2(30, 30), ImVec2(0, 1), ImVec2(1, 0));
		}

		ImGui::SetCursorPos(ImVec2(50, (selectableHeight * count)));
		if (ImGui::Selectable(currentToolDescription.toolName.c_str(), false, ImGuiSelectableFlags_None, ImVec2(ImGui::GetContentRegionAvailWidth(), selectableHeight - 3)))
		{
			returnInfo = &currentToolDescription; 
		}
		ImGui::PopFont();

		// Description -----
		ImGui::SetCursorPosY((selectableHeight * count) + 20);
		ImGui::SetCursorPosX(ImGui::GetCursorPos().x + 52);

		ImGui::PushFont(App->moduleImGui->rudaRegularSmall);
		ImGui::TextWrapped(currentToolDescription.toolDescription.c_str());
		ImGui::PopFont();

		count++;
	}

	return returnInfo; 
}

Texture* ModuleManager::GetIconFromToolType(ToolType toolType)
{
	Texture* toolIconTexture = nullptr;
	switch (toolType)
	{

	case AT_IMAGE:
		toolIconTexture = (Texture*)ResourceManager::getInstance()->GetResource("ImageIcon");
		break;

	default:
		break;
	}

	return toolIconTexture;
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

