#include "ModuleManager.h"
#include "ImageImporter.h"
#include "ResourceManager.h"
#include "MyFileSystem.h"
#include "EmitSoundAction.h"
#include "ModuleWorldManager.h"
#include "Application.h"
#include "ModuleWorldManager.h"
#include "GameViewportDockPanel.h"
#include "ChangeRoomAction.h"
#include "ModuleWindow.h"
#include "ObjectPropertiesDockPanel.h"
#include "Room.h"
#include "ModuleImGui.h"
#include "DisplayTextAction.h"
#include "Texture.h"
#include "ModifyVariableAction.h"
#include "RoomUIHandler.h"
#include "FlyObject.h"
#include "ObjectPropertiesDockPanel.h"
#include "ViewportManager.h"
#include "imgui.h"
#include "RoomDockPanel.h"
#include "FlyVariable.h"

#include "mmgr.h"

ModuleManager::ModuleManager(bool start_enabled)
{
	fakeVarInitAttach = new FlyVariable();
	fakeVarInitAttach->SetDefault(); 

	uiBackgroundTint = float4(1, 1, 1, 1);
}

ModuleManager::~ModuleManager()
{
}

update_status ModuleManager::PostUpdate(float dt)
{
	if (!deleteObjectsFromListTick.empty())
		DeleteObjectsFromListNow();
	
	if (!deleteObjectsTick.empty())
		DeleteObjectsNow(); 

	return UPDATE_CONTINUE;
}

bool ModuleManager::Init()
{
	// Init File System 
	MyFileSystem::getInstance()->Init(); 

	// Load Game Resources -----
	ResourceManager::getInstance()->LoadAllGameResources(); 

	// Tools Descriptions 
	AddToolsNameDescription("Display Image", "This should be the description of the image tool", ACTION_DISPLAY_IMAGE);
	AddToolsNameDescription("Change Scene", "This should be the description of the change scene tool", ACTION_CHANGE_ROOM);
	AddToolsNameDescription("Modify Variable", "This should be the description of the mofdify variable tool", ACTION_MOD_VARIABLE);
	AddToolsNameDescription("Emit Sound", "This should be the description of the emit sound tool", ACTION_EMIT_SOUND);
	AddToolsNameDescription("Display Text", "This should be the description of the display text tool", ACTION_DISPLAY_TEXT);
	AddToolsNameDescription("Display Animation", "This should be the description of the display animation tool", ACTION_DISPLAY_ANIMATION);
	AddToolsNameDescription("Follow Path", "This should be the description of the follow path tool", ACTION_FOLLOW_PATH);
	AddToolsNameDescription("Dialogue", "This should be the description of the dialog tool", ACTION_DIALOGUE);

	return true;
}

bool ModuleManager::Start()
{
	// Load Saved Rooms
	string engineIconPath = MyFileSystem::getInstance()->GetIconsDirectory() + "FlyEngineIcon.bmp";
	App->moduleWindow->SetIcon(engineIconPath.c_str());

	if (App->moduleWorldManager->LoadAllData())
	{
		/*App->moduleRoomManager->SetSelectedRoom(App->moduleRoomManager->GetFirstRoom());
		App->moduleImGui->AddaptToFlySection(FLY_SECTION_ROOM_EDIT);*/
	}

	string initFilePath = MyFileSystem::getInstance()->GetSavedDataDirectory() + "Init.json"; 

	JSON_Value* root = json_parse_file(initFilePath.c_str());
	JSON_Object* root_obj = json_value_get_object(root);

	bool gameMode = json_object_dotget_boolean(root_obj, "InitData.GameMode"); 

	if (gameMode)
	{
		UID firstRoomUID = 0; 
		string projectName = ""; 

		if(json_object_dothas_value(root_obj, "InitData.StartRoomUID"))
			firstRoomUID = json_object_dotget_number(root_obj, "InitData.StartRoomUID"); 
		if (json_object_dothas_value(root_obj, "InitData.ProjectName"))
			projectName = json_object_dotget_string(root_obj, "InitData.ProjectName"); 

		StartGame(firstRoomUID);

		App->SetIsReleaseVersion(true);

		App->moduleWindow->SetTitle(projectName.c_str());
	}
	//else
	//{
	//	JSON_Value* scene_v = json_value_init_object();
	//	JSON_Object* scene_obj = json_value_get_object(scene_v);
	//	json_object_dotset_number(root_obj, "InitData.StartRoomUID", 0);
	//	json_object_dotset_string(root_obj, "InitData.ProjectName", "None");
	//	json_serialize_to_file(scene_v, initFilePath.c_str());
	//}

	return true;
}

void ModuleManager::StartGame(const UID& firstRoomUID)
{
	App->BroadCastEvent(FlyEngineEvent::ENGINE_PLAY); 
	App->isEngineInPlayMode = true; 

	App->moduleWorldManager->SetSelectedRoom(firstRoomUID, true);
	App->moduleImGui->AddaptToFlySection(FLY_SECTION_ROOM_EDIT);
	App->moduleImGui->gameViewportDockPanel->FitViewportToRegion();
	App->moduleWorldManager->FitObjectUtils(); 

	App->SetGameMode(PLAY_MODE);
	App->moduleImGui->AddaptToGameMode();
}

bool ModuleManager::CleanUp()
{
	fakeVarInitAttach->CleanUp();
	delete fakeVarInitAttach; 

	return true;
}

DialogueEditorDockPanel* ModuleManager::GetCurrentDialogueEditor()
{
	DialogueEditorDockPanel* dialogueEditor = (DialogueEditorDockPanel*) App->moduleImGui->GetDockPanel(DockPanelType::DOCK_DIALOGUE_EDITOR);
	return dialogueEditor;
}

int ModuleManager::GetToolsAmount() const
{
	return toolNamesDescriptions.size();
}

ActionSelectableInfo* ModuleManager::DrawActionDictionaryUI(DictionaryPopupFilter popupFilter)
{
	ActionSelectableInfo* returnInfo = nullptr;

	int count = 0;
	int selectableHeight = 42;

	for (auto& currentToolDescription : toolNamesDescriptions)
	{
		// Filter Actions from popup type 
		// UI Button Filter
		if ((popupFilter == FILTER_ACTIONS_UI_BUTTON) &&
			currentToolDescription.actionType == ACTION_DISPLAY_IMAGE)
			continue;

		if (popupFilter == FILTER_ACTIONS_CHARACTER &&
			(currentToolDescription.actionType == ACTION_DISPLAY_IMAGE ||
			 currentToolDescription.actionType == ACTION_DISPLAY_TEXT ||
				currentToolDescription.actionType == ACTION_DISPLAY_ANIMATION))
			continue;

		// Inventory Filter
		if (popupFilter == FILTER_ACTIONS_INVENTORY_CLICK &&
			currentToolDescription.actionType == ACTION_DISPLAY_IMAGE)
			continue;

		// Sequential Filter
		if (popupFilter == FILTER_ACTIONS_FIXED &&
			currentToolDescription.actionType == ACTION_CHANGE_ROOM)
			continue;

		if (popupFilter == FILTER_ACTIONS_FIXED &&
			currentToolDescription.actionType == ACTION_MOD_VARIABLE)
			continue;

		if (popupFilter == FILTER_ACTIONS_FIXED &&
			currentToolDescription.actionType == ACTION_FOLLOW_PATH)
			continue;

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
		ImGui::Text(currentToolDescription.actionDescription.c_str());
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

	case ACTION_DISPLAY_IMAGE:
		toolIconTexture = (Texture*)ResourceManager::getInstance()->GetResource("ImageIcon");
		break;

	case ACTION_CHANGE_ROOM:
		toolIconTexture = (Texture*)ResourceManager::getInstance()->GetResource("ChangeScene");
		break;

	case ACTION_MOD_VARIABLE:
		toolIconTexture = (Texture*)ResourceManager::getInstance()->GetResource("WrenchIcon");
		break;

	case ACTION_EMIT_SOUND:
		toolIconTexture = (Texture*)ResourceManager::getInstance()->GetResource("SpeakerIcon");
		break;

	case ACTION_DISPLAY_TEXT:
		toolIconTexture = (Texture*)ResourceManager::getInstance()->GetResource("DisplayTextIcon");
		break;

	case ACTION_DISPLAY_ANIMATION:
		toolIconTexture = (Texture*)ResourceManager::getInstance()->GetResource("AnimationIcon");
		break;

	case ACTION_FOLLOW_PATH:
		toolIconTexture = (Texture*)ResourceManager::getInstance()->GetResource("PathIcon");
		break;

	case ACTION_DIALOGUE:
		toolIconTexture = (Texture*)ResourceManager::getInstance()->GetResource("DialogueIcon");
		break;

	default:
		break;
	}

	return toolIconTexture;
}

void ModuleManager::NotifyVariableChange(FlyVariable* variableCurrentValue)
{
	for (auto& currentRoom : App->moduleWorldManager->GetRoomsInWorldList())
	{
		currentRoom->NotifyVariableChange(variableCurrentValue);
	}
}

float4 ModuleManager::GetSceneUITint()
{
	return uiBackgroundTint; 
}

void ModuleManager::SetSceneUITint(float4 _colorTint)
{
	uiBackgroundTint = _colorTint;
}

void ModuleManager::AddDeleteFromListObject(FlyObject* newObjectToDelete)
{
	if (newObjectToDelete != nullptr)
		deleteObjectsFromListTick.push_back(newObjectToDelete);
}

void ModuleManager::AddDeleteObject(FlyObject* newObjectToDelete)
{
	if (newObjectToDelete != nullptr)
		deleteObjectsTick.push_back(newObjectToDelete);
}

void ModuleManager::DeleteObjectsNow()
{
	for (auto& currentObject : deleteObjectsTick)
	{
		Room* containerRoom = currentObject->GetParentRoom();

		if (containerRoom != nullptr)
		{
			containerRoom->DeleteFlyObjectFromList(currentObject);
		
			currentObject->CleanUp();
			delete currentObject;
			currentObject = nullptr;
		}
	}

	deleteObjectsTick.clear(); 
}

void ModuleManager::DeleteObjectsFromListNow()
{
	for (auto& currentObject : deleteObjectsFromListTick)
	{
		Room* containerRoom = currentObject->GetParentRoom();
		containerRoom->DeleteFlyObjectFromList(currentObject);
	}


	deleteObjectsFromListTick.clear();
}

void ModuleManager::DrawActionListWithSettings(FlyObject* ownerObejct)
{
	if (ownerObejct->IsInventoryItem())
	{
		ImGui::PushFont(App->moduleImGui->rudaBlackBig);
		ImGui::Text("Actions When Picked Up: ");
		ImGui::PopFont();
	}
	else
	{
		ImGui::PushFont(App->moduleImGui->rudaBlackBig);
		ImGui::Text("Object Actions: ");
		ImGui::PopFont();
	}

	ImGui::SameLine(); 
	string buttonName = "Add Action##Add" + to_string(ownerObejct->GetUID());
	if (ImGui::Button(buttonName.c_str()))
	{
		ImGui::OpenPopup("SelectAction");
	}

	if (ImGui::BeginPopup("SelectAction"))
	{
		ActionSelectableInfo* selectableInfo = DrawActionDictionaryUI(FILTER_ACTIONS_INVENTORY_CLICK); 
		
		if (selectableInfo != nullptr)
		{
			switch (selectableInfo->actionType)
			{
			case ACTION_MOD_VARIABLE:
			{
				ModifyVariableAction* modVarAction = ownerObejct->AddModifyVariableAction();
				modVarAction->SetOccObjectClicked(true); 
			}
				break;

			case ACTION_EMIT_SOUND:
			{
				EmitSoundAction* emitSoundAction =  ownerObejct->AddEmitSoundAction();
				emitSoundAction->SetOccObjectClicked(true);
			}
				break;

			case ACTION_CHANGE_ROOM:
			{
				ChangeRoomAction* changeRoomAction = ownerObejct->AddChangeRoomAction();
				changeRoomAction->SetOccObjectClicked(true);
			}

			case ACTION_DISPLAY_TEXT:
			{
				DisplayTextAction* displayTextAction = ownerObejct->AddDisplayTextAction();
				displayTextAction->SetOccObjectClicked(true);
			}
				break;

			}
		}
		ImGui::EndPopup();
	}
	ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0.12f, 0.14f, 0.17f, 1.00f));
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(5, 5));

	ImGui::BeginChild("##ToolsListObjectProperties", ImVec2(ImGui::GetContentRegionAvailWidth(), 200));

	int count = 0;
	ObjectPropertiesDockPanel* objectProperties = nullptr;
	objectProperties = (ObjectPropertiesDockPanel*)App->moduleImGui->GetDockPanel(DOCK_OBJECT_PROPERTIES);
	for (auto& currentAction : ownerObejct->GetActionsList())
	{
		if (ownerObejct->IsInventoryItem() && currentAction->GetType() == ACTION_DISPLAY_IMAGE)
			continue; 

		ActionSelectableInfo selectableInfo = currentAction->GetActionSelectableInfo();

		if (objectProperties->DrawActionSelectable(selectableInfo, currentAction, count, 40))
		{
			ownerObejct->SetSelectedAction(selectableInfo.actionType);
		}

		count++;
	}

	ImGui::EndChild();

	ImGui::PopStyleVar();
	ImGui::PopStyleColor();

	if (ownerObejct->GetSelectedAction())
		ownerObejct->GetSelectedAction()->DrawUISettingsInButton();
}

void ModuleManager::DrawImageFitInCenter(Texture* textureToShow)
{
	if (textureToShow == nullptr)
		return; 

	ImVec2 imageMaxSize = ImVec2(ImGui::GetContentRegionAvail().x - 20, ImGui::GetContentRegionAvail().y - 20);
	ImVec2 uiImageDimensions = ImVec2(0,0);

	if (textureToShow->IsVertical())
	{
		uiImageDimensions.y = imageMaxSize.y; 
		uiImageDimensions.x = uiImageDimensions.y * textureToShow->GetAspectRatio();

		if (uiImageDimensions.x >= imageMaxSize.x)
		{
			float diff = uiImageDimensions.x - imageMaxSize.x;
			uiImageDimensions.x -= diff;
			uiImageDimensions.y = uiImageDimensions.x * textureToShow->GetAspectRatio();
		}
	}
	else
	{
		uiImageDimensions.x = imageMaxSize.x;
		uiImageDimensions.y = uiImageDimensions.x / textureToShow->GetAspectRatio();

		if (uiImageDimensions.y >= imageMaxSize.y)
		{
			float diff = uiImageDimensions.y - imageMaxSize.y;
			uiImageDimensions.y -= diff;
			uiImageDimensions.x = uiImageDimensions.y * textureToShow->GetAspectRatio();
		}
	}

	ImGui::SetCursorPos(ImVec2(ImGui::GetContentRegionAvailWidth() / 2 - (uiImageDimensions.x / 2), ImGui::GetContentRegionAvail().y / 2 - (uiImageDimensions.y / 2)));
	ImGui::Image((ImTextureID)textureToShow->GetTextureID(), uiImageDimensions);

	/*ImVec2 imageMaxSize = ImVec2(ImGui::GetContentRegionAvailWidth(), 135);
	ImVec2 uiImageDimensions = ImVec2(150, 150);
	Texture* mainButtonTexture = textureToShow;
	ImTextureID mainTextureID = 0;

	if (mainButtonTexture != nullptr)
	{
		float aspectRatio = mainButtonTexture->GetAspectRatio();

		if (mainButtonTexture->IsVertical())
		{
			uiImageDimensions.y = imageMaxSize.y;
			uiImageDimensions.x = uiImageDimensions.x * aspectRatio;
		}
		else
		{
			uiImageDimensions.y = imageMaxSize.y;
			uiImageDimensions.x = uiImageDimensions.y * aspectRatio;

			if (uiImageDimensions.x > imageMaxSize.x)
			{
				float diff = uiImageDimensions.x - imageMaxSize.x;
				uiImageDimensions.x -= diff;
				uiImageDimensions.y = uiImageDimensions.x * aspectRatio;
			}
		}
		mainTextureID = (ImTextureID)mainButtonTexture->GetTextureID();
	}

	ImGui::SetCursorPos(ImVec2(ImGui::GetContentRegionAvailWidth() / 2 - (uiImageDimensions.x / 2), imageMaxSize.y / 2 - (uiImageDimensions.y / 2) + 10));
	ImGui::Image(mainTextureID, uiImageDimensions);*/
}

FlyObject* ModuleManager::GetSelectedFlyObject()
{
	return App->moduleWorldManager->GetSelectedRoom()->GetSelectedObject();
}

void ModuleManager::SetSelectedFlyObject(FlyObject* newSelectedObject)
{
	App->moduleWorldManager->GetSelectedRoom()->SetSelectedObject(newSelectedObject);
}

UI_Element* ModuleManager::GetSelectedUIElement()
{
	return App->moduleWorldManager->GetSelectedRoom()->roomUIHandler->GetSelectedElement(); 
}

void ModuleManager::SetSelectedUIElement(UI_Element* newSelectedObject)
{
	App->moduleWorldManager->GetSelectedRoom()->roomUIHandler->SetSelectedElement(newSelectedObject); 
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

