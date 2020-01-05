#include "ModuleManager.h"

#include "ImageImporter.h"
#include "ResourceManager.h"
#include "MyFileSystem.h"
#include "Application.h"
#include "ModuleRoomManager.h"
#include "Room.h"
#include "ModuleImGui.h"
#include "Texture.h"
#include "ModifyVariableAction.h"
#include "FlyObject.h"
#include "ViewportManager.h"
#include "imgui.h"
#include "RoomDockPanel.h"
#include "FlyVariable.h"

#include "mmgr.h"

ModuleManager::ModuleManager(bool start_enabled)
{
	fakeVarInitAttach = new FlyVariable();
	fakeVarInitAttach->SetDefault(); 
}

ModuleManager::~ModuleManager()
{
}

bool ModuleManager::Init()
{
	// Init File System 
	MyFileSystem::getInstance()->Init(); 

	// Load Engine Icons -----
	LoadEngineIcons();
	ResourceManager::getInstance()->LoadAllGameResources(); 

	// Tools Descriptions 
	AddToolsNameDescription("Display Image", "This should be the description of the image tool", AT_DISPLAY_IMAGE);
	AddToolsNameDescription("Change Scene", "This should be the description of the change scene tool", AT_CHANGE_ROOM);
	AddToolsNameDescription("Modify Variable", "This should be the description of the mofdify variable tool", AT_MOD_VARIABLE);

	return true;
}

bool ModuleManager::Start()
{
	// Load Saved Rooms
	if (App->moduleRoomManager->LoadRoomsData())
	{
		App->moduleRoomManager->SetSelectedRoom(App->moduleRoomManager->GetFirstRoom());
		App->moduleImGui->AddaptToFlySection(FLY_SECTION_ROOM_EDIT);
	}

	//// Create Debug Object 
	//Room* selectedRoom = App->moduleRoomManager->GetSelectedRoom();
	//FlyObject* addingObject = selectedRoom->CreateFlyObject("AddingObject", "TráTrá!");
	//string keyPath = MyFileSystem::getInstance()->GetSolutionDirectory() + "\\EngineResources\\Images\\Key.png";

	//addingObject->AddDisplayImageAction(keyPath.c_str());
	//addingObject->CreateClickableArea(float2(0, 0), float2(1, 1), false);
	//addingObject->clickableAreaActive = true;

	//ModifyVariableAction* modVarAction = addingObject->AddModifyVariableAction(); 
	//ModifyVariableEffect* newEffect = modVarAction->AddEmptyEffect(); 

	//Blackboard* roomBB = App->moduleRoomManager->GetSelectedRoom()->GetBlackboard(); 
	//newEffect->AttachToVariable(roomBB->GetVariable("Default_Var"));

	//// Add Condition
	//ActionCondition* actionCondition = modVarAction->AddEmptyCondition();
	//actionCondition->targetVariable = roomBB->GetVariable("Default_Var");
	//actionCondition->actionConditionOperator = AC_EQUALS_TO; 
	//actionCondition->targetValueInteger = 2; 

	return true;
}

bool ModuleManager::CleanUp()
{
	fakeVarInitAttach->CleanUp();
	delete fakeVarInitAttach; 

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

	Texture* scaleIcon = ImageImporter::getInstance()->LoadTexture(string(MyFileSystem::getInstance()->GetIconsDirectory() + "ScaleIcon.png"), true);
	ResourceManager::getInstance()->AddResource((Resource*)scaleIcon, "ScaleIcon");

	Texture* wrenchIcon = ImageImporter::getInstance()->LoadTexture(string(MyFileSystem::getInstance()->GetIconsDirectory() + "WrenchIcon.png"), true);
	ResourceManager::getInstance()->AddResource((Resource*)wrenchIcon, "WrenchIcon");

	Texture* exportIcon = ImageImporter::getInstance()->LoadTexture(string(MyFileSystem::getInstance()->GetIconsDirectory() + "ExportIcon.png"), false);
	ResourceManager::getInstance()->AddResource((Resource*)exportIcon, "ExportIcon");

	Texture* importIcon = ImageImporter::getInstance()->LoadTexture(string(MyFileSystem::getInstance()->GetIconsDirectory() + "ImportIcon.png"), false);
	ResourceManager::getInstance()->AddResource((Resource*)importIcon, "ImportIcon");

	Texture* stopIcon = ImageImporter::getInstance()->LoadTexture(string(MyFileSystem::getInstance()->GetIconsDirectory() + "StopIcon.png"), false);
	ResourceManager::getInstance()->AddResource((Resource*)stopIcon, "StopIcon");

	Texture* changeScene = ImageImporter::getInstance()->LoadTexture(string(MyFileSystem::getInstance()->GetIconsDirectory() + "ChangeScene.png"), true);
	ResourceManager::getInstance()->AddResource((Resource*)changeScene, "ChangeScene");

	Texture* emptyObject = ImageImporter::getInstance()->LoadTexture(string(MyFileSystem::getInstance()->GetIconsDirectory() + "EmptyObject.png"), true);
	ResourceManager::getInstance()->AddResource((Resource*)emptyObject, "EmptyObject");

	Texture* inventoryItemIcon = ImageImporter::getInstance()->LoadTexture(string(MyFileSystem::getInstance()->GetIconsDirectory() + "InventoryItemIcon.png"), false);
	ResourceManager::getInstance()->AddResource((Resource*)inventoryItemIcon, "InventoryItemIcon");

	Texture* toggleIcon = ImageImporter::getInstance()->LoadTexture(string(MyFileSystem::getInstance()->GetIconsDirectory() + "ToggleIcon.png"), false);
	ResourceManager::getInstance()->AddResource((Resource*)toggleIcon, "ToggleIcon");

	Texture* naturalNumberIcon = ImageImporter::getInstance()->LoadTexture(string(MyFileSystem::getInstance()->GetIconsDirectory() + "NaturalNumberIcon.png"), false);
	ResourceManager::getInstance()->AddResource((Resource*)naturalNumberIcon, "NaturalNumberIcon");

	Texture* addOperatorIcon = ImageImporter::getInstance()->LoadTexture(string(MyFileSystem::getInstance()->GetIconsDirectory() + "AddOperatorIcon.png"), false);
	ResourceManager::getInstance()->AddResource((Resource*)addOperatorIcon, "AddOperatorIcon");

	Texture* multiplyOperatorIcon = ImageImporter::getInstance()->LoadTexture(string(MyFileSystem::getInstance()->GetIconsDirectory() + "MultiplyOperatorIcon.png"), false);
	ResourceManager::getInstance()->AddResource((Resource*)multiplyOperatorIcon, "MultiplyOperatorIcon");

	Texture* substractyOperatorIcon = ImageImporter::getInstance()->LoadTexture(string(MyFileSystem::getInstance()->GetIconsDirectory() + "SubstractOperatorIcon.png"), false);
	ResourceManager::getInstance()->AddResource((Resource*)substractyOperatorIcon, "SubstractOperatorIcon");

	Texture* divideOperatorIcon = ImageImporter::getInstance()->LoadTexture(string(MyFileSystem::getInstance()->GetIconsDirectory() + "DivideOperatorIcon.png"), false);
	ResourceManager::getInstance()->AddResource((Resource*)divideOperatorIcon, "DivideOperatorIcon");

	Texture* equalOperatorIcon = ImageImporter::getInstance()->LoadTexture(string(MyFileSystem::getInstance()->GetIconsDirectory() + "EqualOperatorIcon.png"), false);
	ResourceManager::getInstance()->AddResource((Resource*)equalOperatorIcon, "EqualOperatorIcon");

	Texture* toggleOperatorIcon = ImageImporter::getInstance()->LoadTexture(string(MyFileSystem::getInstance()->GetIconsDirectory() + "ToggleOperatorIcon.png"), false);
	ResourceManager::getInstance()->AddResource((Resource*)toggleOperatorIcon, "ToggleOperatorIcon");

	Texture* pngIcon = ImageImporter::getInstance()->LoadTexture(string(MyFileSystem::getInstance()->GetIconsDirectory() + "PNGIcon.png"), false);
	ResourceManager::getInstance()->AddResource((Resource*)pngIcon, "PNGIcon");

	Texture* jpgIcon = ImageImporter::getInstance()->LoadTexture(string(MyFileSystem::getInstance()->GetIconsDirectory() + "JPGIcon.png"), false);
	ResourceManager::getInstance()->AddResource((Resource*)jpgIcon, "JPGIcon");

	Texture* folderIcon = ImageImporter::getInstance()->LoadTexture(string(MyFileSystem::getInstance()->GetIconsDirectory() + "FolderIcon.png"), false);
	ResourceManager::getInstance()->AddResource((Resource*)folderIcon, "FolderIcon");

	Texture* speakerIcon = ImageImporter::getInstance()->LoadTexture(string(MyFileSystem::getInstance()->GetIconsDirectory() + "SpeakerIcon.png"), false);
	ResourceManager::getInstance()->AddResource((Resource*)speakerIcon, "SpeakerIcon");
}

int ModuleManager::GetToolsAmount() const
{
	return toolNamesDescriptions.size();
}

ActionSelectableInfo* ModuleManager::DrawActionDictionaryUI()
{
	ActionSelectableInfo* returnInfo = nullptr; 

	int count = 0; 
	int selectableHeight = 42;

	for (auto& currentToolDescription : toolNamesDescriptions)
	{
		ImGui::PushFont(App->moduleImGui->rudaBoldMid);

		Texture* iconTexture = GetIconFromActionType(currentToolDescription.actionType); 
		ImGui::SetCursorPos(ImVec2(10, 5 + (selectableHeight * count)));
		if (iconTexture) {
			ImGui::Image((ImTextureID)iconTexture->GetTextureID(), ImVec2(30, 30), ImVec2(0, 1), ImVec2(1, 0));
		}
		else
		{
			ImGui::Image(0, ImVec2(30, 30), ImVec2(0, 1), ImVec2(1, 0));
		}

		ImGui::SetCursorPos(ImVec2(50, (selectableHeight * count)));
		if (ImGui::Selectable(currentToolDescription.actionName.c_str(), false, ImGuiSelectableFlags_None, ImVec2(ImGui::GetContentRegionAvailWidth(), selectableHeight - 3)))
		{
			returnInfo = &currentToolDescription; 
		}
		ImGui::PopFont();

		// Description -----
		ImGui::SetCursorPosY((selectableHeight * count) + 20);
		ImGui::SetCursorPosX(ImGui::GetCursorPos().x + 52);

		ImGui::PushFont(App->moduleImGui->rudaRegularSmall);
		ImGui::TextWrapped(currentToolDescription.actionDescription.c_str());
		ImGui::PopFont();

		count++;
	}

	return returnInfo; 
}

Texture* ModuleManager::GetIconFromActionType(ActionType toolType)
{
	Texture* toolIconTexture = nullptr;
	switch (toolType)
	{

	case AT_DISPLAY_IMAGE:
		toolIconTexture = (Texture*)ResourceManager::getInstance()->GetResource("ImageIcon");
		break;

	case AT_CHANGE_ROOM:
		toolIconTexture = (Texture*)ResourceManager::getInstance()->GetResource("ChangeScene");
		break;

	case AT_MOD_VARIABLE:
		toolIconTexture = (Texture*)ResourceManager::getInstance()->GetResource("WrenchIcon");
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

std::list<ActionSelectableInfo> ModuleManager::GetToolsNamesDescriptionsList() const
{
	return toolNamesDescriptions;
}

void ModuleManager::AddToolsNameDescription(std::string name, std::string description, ActionType newTooltype)
{
	ActionSelectableInfo newtsi; 

	newtsi.actionName = name; 
	newtsi.actionDescription = description; 
	newtsi.actionType = newTooltype; 

	toolNamesDescriptions.push_back(newtsi);
}

ActionSelectableInfo ModuleManager::GetToolNameDescription(std::string toolCheckName) const
{
	for (auto& it : toolNamesDescriptions)
	{
		if ((it).actionName == toolCheckName)
			return (it);
	}
}

ActionSelectableInfo ModuleManager::GetToolNameDescription(int pos) const
{
	int count = 0;
	for (auto& it : toolNamesDescriptions)
	{	
		if (count == pos)
			return (it); 

		count++;
	}

	return ActionSelectableInfo();
}

