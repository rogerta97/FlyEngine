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

		ImGui::PushFont(App->moduleImGui->rudaRegularTiny);
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

