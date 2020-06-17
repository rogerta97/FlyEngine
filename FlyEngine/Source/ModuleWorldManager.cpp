#include "ModuleWorldManager.h"
#include "ModuleInput.h"
#include "Application.h"
#include "ModuleImGui.h"
#include "Blackboard.h"
#include "RandomNumberGenerator.h"
#include "GamePropertiesDockPanel.h"
#include "ViewportManager.h"
#include "GameInventory.h"
#include "RoomUIHandler.h"
#include "AudioImporter.h"
#include "ModuleAudioManager.h"
#include "Room.h"
#include "NodeGraph.h"
#include "FontImporter.h"
#include "ImageImporter.h"
#include "MusicTrack.h"
#include "GameViewportDockPanel.h"
#include "ResourceManager.h"
#include "MyFileSystem.h"
#include "FlyObject.h"
#include "Texture.h"
#include "TextureMSAA.h"
#include "SaveAndLoad.h"

#include "mmgr.h"

ModuleWorldManager::ModuleWorldManager(bool start_enabled)
{
	moduleType = MODULE_ENGINE_MANAGER;
	connectionsInWorldAmount = 0;
	roomsInWoldAmount = 0;
}

ModuleWorldManager::~ModuleWorldManager()
{
}

bool ModuleWorldManager::Start()
{
	globalBlackboard = new Blackboard(); 

	if (!App->isEngineInPlayMode)
	{
		autosaveTimer = new Timer();
		autosaveTimer->Start(); 
	}

	return true;
}

update_status ModuleWorldManager::PreUpdate(float dt)
{
	return UPDATE_CONTINUE;
}

update_status ModuleWorldManager::Update(float dt)
{
	if (!App->isEngineInPlayMode)
	{
		if (autosaveTimer->Read() > autoSaveThreshold)
		{
			autosaveTimer->Restart();
			AutoSave();
		}
	}

	if (App->moduleInput->GetKey(SDL_SCANCODE_L) == KEY_DOWN)
	{
		GameInventory::getInstance()->returnDroppingObject = true;
		GameInventory::getInstance()->CheckReturnDroppingObject();
	}

	for (auto& currentRoom : roomsInWorldList)
	{
		currentRoom->Update(); 
	}

	if(App->isEngineInPlayMode && GameInventory::getInstance()->IsOpened())
		HandleSlotDrag();

	if (App->moduleImGui->gameViewportDockPanel->initialized && !firstFit)
	{
		firstFit = true; 
		FitObjectUtils(); 
		FitUIObjectUtils();	
	}

	return UPDATE_CONTINUE;
}

void ModuleWorldManager::AutoSave()
{
	if (App->flySection == FlyEngineSection::FLY_SECTION_WORLD_EDITOR)
	{
		flog("World Graph Saved");
	}
	else if (App->flySection == FlyEngineSection::FLY_SECTION_ROOM_EDIT)
	{
		if (selectedRoom != nullptr)
		{
			SaveAndLoad::getInstance()->SaveRoomData(selectedRoom);
			flog("%s Room Saved", selectedRoom->GetName().c_str());
		}
	}

	SaveAndLoad::getInstance()->SaveInitFile(); 
}

void ModuleWorldManager::FitUIObjectUtils()
{
	for (auto& currentRoom : roomsInWorldList)
	{
		for (auto& uiElement : currentRoom->roomUIHandler->uiElements)
		{
			uiElement->GetHolderObject()->FitObjectUtils();
		}
	}
}

void ModuleWorldManager::ResetSequentials()
{
	
}

update_status ModuleWorldManager::PostUpdate(float dt)
{


	return UPDATE_CONTINUE;
}

bool ModuleWorldManager::CleanUp()
{	
	CleanUpRooms();
	DeleteSingletones();

	globalBlackboard->CleanUp(); 
	delete globalBlackboard;

	return true;
}

void ModuleWorldManager::HandleSlotDrag()
{
	if (!GameInventory::getInstance()->GetBackgroundBB()->IsMouseOver())
		return; 

	static int slotClicked = -1; 
	if (ImGui::IsMouseClicked(0))
	{
		canDragInventory = true;
	
		slotClicked = GetSlotClickedID();
		flog("Slot Clicked: %d", slotClicked);
		flog("CAN DRAG");
	}

	if (ImGui::IsMouseReleased(0))
	{
		canDragInventory = false;
		slotClicked = -1;
		flog("CAN NOT DRAG");
		
	}

	if (canDragInventory && slotClicked != -1 && ImGui::IsMouseDragPastThreshold(0, 15.0f))
	{
		flog("Dragging");
		GameInventory::getInstance()->PickObjectFromInventory(slotClicked);
	}
}

int ModuleWorldManager::GetSlotClickedID()
{
	bool slotOver = false; 
	int slotIndex = 0;
	for (auto& currentslot : *GameInventory::getInstance()->GetInventorySlots())
	{
		if (currentslot->GetSlotBB()->IsMouseOver())
		{
			slotOver = true;
			return slotIndex; 
		}

		slotIndex++;
	}

	return -1; 
}

void ModuleWorldManager::SaveConfigData()
{
	// Save Inventory Colors
	JSON_Value* scene_v = json_value_init_object();
	JSON_Object* scene_obj = json_value_get_object(scene_v);

	json_object_dotset_number(scene_obj, "Inventory.BackgroundColor.r", GameInventory::getInstance()->backgroundColor.x);
	json_object_dotset_number(scene_obj, "Inventory.BackgroundColor.g", GameInventory::getInstance()->backgroundColor.y);
	json_object_dotset_number(scene_obj, "Inventory.BackgroundColor.b", GameInventory::getInstance()->backgroundColor.z);
	json_object_dotset_number(scene_obj, "Inventory.BackgroundColor.a", GameInventory::getInstance()->backgroundColor.w);

	json_object_dotset_number(scene_obj, "Inventory.SlotColor.r", GameInventory::getInstance()->slotColor.x);
	json_object_dotset_number(scene_obj, "Inventory.SlotColor.g", GameInventory::getInstance()->slotColor.y);
	json_object_dotset_number(scene_obj, "Inventory.SlotColor.b", GameInventory::getInstance()->slotColor.z);
	json_object_dotset_number(scene_obj, "Inventory.SlotColor.a", GameInventory::getInstance()->slotColor.w);

	json_object_dotset_number(scene_obj, "Inventory.ArrowColor.r", GameInventory::getInstance()->arrowColor.x);
	json_object_dotset_number(scene_obj, "Inventory.ArrowColor.g", GameInventory::getInstance()->arrowColor.y);
	json_object_dotset_number(scene_obj, "Inventory.ArrowColor.b", GameInventory::getInstance()->arrowColor.z);
	json_object_dotset_number(scene_obj, "Inventory.ArrowColor.a", GameInventory::getInstance()->arrowColor.w);

	string savePath = MyFileSystem::getInstance()->GetSavedDataDirectory() + "Config.json"; 
	json_serialize_to_file(scene_v, savePath.c_str());
}

void ModuleWorldManager::FitObjectUtils()
{
	if (selectedRoom != nullptr)
		selectedRoom->FitObjectUtils(); 
}

void ModuleWorldManager::DeleteSingletones()
{
	GameInventory::getInstance()->CleanUp();
	MyFileSystem::getInstance()->Delete();
	RandomNumberGenerator::getInstance()->Delete();
	ImageImporter::getInstance()->Delete();
	AudioImporter::getInstance()->Delete(); 
	SaveAndLoad::getInstance()->Delete();
	ResourceManager::getInstance()->CleanUp();
	ViewportManager::getInstance()->Delete();
	FontImporter::getInstance()->Delete(); 
}

void ModuleWorldManager::ReceiveEvent(FlyEngineEvent eventType)
{
	switch (eventType)
	{
	case FlyEngineEvent::ENGINE_PLAY:
		
		if (App->flySection == FlyEngineSection::FLY_SECTION_ROOM_EDIT && GetSelectedRoom() != nullptr)
		{
			ViewportManager::getInstance()->drawGizmos = false; 
			GameInventory::getInstance()->CloseInventory(false);
		}

		break; 

	case FlyEngineEvent::ENGINE_STOP:

		if (App->flySection == FlyEngineSection::FLY_SECTION_ROOM_EDIT && GetSelectedRoom() != nullptr)
		{
			ViewportManager::getInstance()->drawGizmos = true;
			GameInventory::getInstance()->ClearItems();
		}

		if (App->moduleAudioManager->currentMusic)
		{
			App->moduleAudioManager->currentMusic->Stop();
			App->moduleAudioManager->currentMusic = nullptr; 
		}

		for (auto& currentRoom : roomsInWorldList)
		{
			if (currentRoom->reloadWhenStop)
			{
				currentRoom->ReloadRoom();
				flog("Room Reloaded: %s", currentRoom->GetName().c_str()); 
				currentRoom->reloadWhenStop = false; 
			}
		}

		break; 

	case FlyEngineEvent::ENTER_ROOM:

		// Play On Scene Enter Events
		if (App->flySection == FlyEngineSection::FLY_SECTION_ROOM_EDIT && App->isEngineInPlayMode && GetSelectedRoom() != nullptr)
		{
			for (auto& currentObject : GetSelectedRoom()->objectsInRoom)
			{
				currentObject->OnSceneEnter(); 
			}

			if (!GetSelectedRoom()->reloadWhenStop)
			{
				GetSelectedRoom()->reloadWhenStop = true;
				flog("Needs To Reload: %s", GetSelectedRoom()->GetName().c_str()); 
			}

			if (GetSelectedRoom()->backgroundMusic != nullptr)
			{
				if (App->moduleAudioManager->currentMusic == nullptr)
				{
					App->moduleAudioManager->currentMusic = GetSelectedRoom()->backgroundMusic; 
					App->moduleAudioManager->currentMusic->Play(0, true);
				}	
				else if (App->moduleAudioManager->currentMusic != GetSelectedRoom()->backgroundMusic)
				{
					App->moduleAudioManager->currentMusic = GetSelectedRoom()->backgroundMusic;
					App->moduleAudioManager->currentMusic->Play(0, true);
				}
			}
			else
			{
				App->moduleAudioManager->currentMusic->Stop();
				App->moduleAudioManager->currentMusic = nullptr;
			}
		}

		break;

	case FlyEngineEvent::LEAVE_ROOM:

		// Play On Scene Enter Events
		if (App->flySection == FlyEngineSection::FLY_SECTION_ROOM_EDIT && App->isEngineInPlayMode && GetSelectedRoom() != nullptr)
		{
			for (auto& currentObject : GetSelectedRoom()->objectsInRoom)
			{
				currentObject->OnSceneLeave();
			}
		}

		break;
	}
}

bool ModuleWorldManager::LoadAllData()
{
	string roomsDirectory = MyFileSystem::getInstance()->GetSavedDataDirectory() + "RoomsData";
	vector<string> roomsSavedFiles;
	MyFileSystem::getInstance()->GetFilesInDirectory(roomsDirectory.c_str(), roomsSavedFiles, false);

	// Load Global Blackboard 
	App->moduleWorldManager->globalBlackboard->LoadData("Global"); 

	// Load Rooms -------
	for (auto& currentRoomFile : roomsSavedFiles)
	{
		JSON_Value* root = json_parse_file(string(roomsDirectory + "\\" + currentRoomFile).c_str());
		JSON_Object* root_obj = json_value_get_object(root);

		string currentRoomName = json_object_dotget_string(root_obj, "RoomData.Name");
		Room* newRoom = App->moduleWorldManager->CreateEmptyRoom(currentRoomName);

		string uidStr = string("RoomData.UID");
		newRoom->SetUID(json_object_dotget_number(root_obj, uidStr.c_str()));
	}

	for (auto& currentRoom : roomsInWorldList)
	{
		SaveAndLoad::getInstance()->LoadDataToRoom(roomsDirectory + "\\" + currentRoom->GetName() + ".json", currentRoom);

		// Load Thumbnail 
		string thumbnailStr = MyFileSystem::getInstance()->GetThumbnilesDirectory() + "\\" + to_string((int)currentRoom->GetUID()) + "_Thumbnail";
		Texture* newThumbnail = ImageImporter::getInstance()->LoadTexture (thumbnailStr.c_str(), false);
	}


	// Load Inventory Style
	JSON_Value* root = json_parse_file(std::string(MyFileSystem::getInstance()->GetSavedDataDirectory() + "Config.json").c_str());
	JSON_Object* scene_obj = json_value_get_object(root);

	GameInventory::getInstance()->backgroundColor.x = json_object_dotget_number(scene_obj, "Inventory.BackgroundColor.r");
	GameInventory::getInstance()->backgroundColor.y = json_object_dotget_number(scene_obj, "Inventory.BackgroundColor.g");
	GameInventory::getInstance()->backgroundColor.z = json_object_dotget_number(scene_obj, "Inventory.BackgroundColor.b");
	GameInventory::getInstance()->backgroundColor.w = json_object_dotget_number(scene_obj, "Inventory.BackgroundColor.a");
				   
	GameInventory::getInstance()->slotColor.x = json_object_dotget_number(scene_obj, "Inventory.SlotColor.r");
	GameInventory::getInstance()->slotColor.y = json_object_dotget_number(scene_obj, "Inventory.SlotColor.g");
	GameInventory::getInstance()->slotColor.z = json_object_dotget_number(scene_obj, "Inventory.SlotColor.b");
	GameInventory::getInstance()->slotColor.w = json_object_dotget_number(scene_obj, "Inventory.SlotColor.a");
				   
	GameInventory::getInstance()->arrowColor.x = json_object_dotget_number(scene_obj, "Inventory.ArrowColor.r");
	GameInventory::getInstance()->arrowColor.y = json_object_dotget_number(scene_obj, "Inventory.ArrowColor.g");
	GameInventory::getInstance()->arrowColor.z = json_object_dotget_number(scene_obj, "Inventory.ArrowColor.b");
	GameInventory::getInstance()->arrowColor.w = json_object_dotget_number(scene_obj, "Inventory.ArrowColor.a");

	if (roomsSavedFiles.size() > 0)
		return true; 

	return false; 
}

Room* ModuleWorldManager::CreateEmptyRoom(string roomName)
{
	Room* newRoom = new Room(roomName);
	roomsInWorldList.push_back(newRoom);
	roomsInWoldAmount++;
	return newRoom;
}

void ModuleWorldManager::DeleteRoom(string roomName)
{
	UID roomUID = GetRoom(roomName)->GetUID();

	if (selectedRoom->GetUID() == roomUID) {
		selectedRoom = nullptr; 
	}

	DeleteRoom(roomUID);	
}

void ModuleWorldManager::DeleteRoom(UID roomID)
{
	for (auto it = roomsInWorldList.begin(); it != roomsInWorldList.end(); it++) {

		if ((*it)->GetUID() == roomID) 
		{
			// Clean Room Residual Files 
			//(*it)->DeleteRoomFiles(); 

			// Clean Room 
			(*it)->BreakOutputConnections();
			(*it)->BreakEnterConnections(); 
			(*it)->CleanUp(); 
			delete (*it);
			roomsInWorldList.erase(it);
			break;
		}
	}

	roomsInWoldAmount--;
}

void ModuleWorldManager::DeleteRoom(Room* roomToDelete)
{
	DeleteRoom(roomToDelete->GetUID()); 
}

void ModuleWorldManager::CleanUpRooms()
{
	for (auto it = roomsInWorldList.begin(); it != roomsInWorldList.end(); it++) {
		(*it)->CleanUp();
		delete (*it);
	}

	roomsInWorldList.clear();
	roomsInWoldAmount = 0;
}

list<Room*>& ModuleWorldManager::GetRoomsInWorldList()
{
	return roomsInWorldList; 
}

void ModuleWorldManager::SerializeRoomListNames()
{
	std::string saveFilePath = MyFileSystem::getInstance()->GetSavedDataDirectory() + "RoomNames.json";

	JSON_Value* scene_v = json_value_init_object();
	JSON_Object* scene_obj = json_value_get_object(scene_v);

	for (auto& it : roomsInWorldList)
	{
	//	string saveNameString = "RoomNamesList." + it->GetName().c_str(); 
	//	json_object_dotset_number(scene_obj, string(roomToLoad->GetName().c_str() + string(".ObjectsAmount")).c_str());
	}

	json_serialize_to_file(scene_v, saveFilePath.c_str());
}

const char** ModuleWorldManager::GetRoomsAsCombo(bool includeSelected)
{
	const char* roomsNameRet[256];
	
	int count = 0; 
	for (auto& it : roomsInWorldList) {
		roomsNameRet[count++] = it->GetName().c_str(); 
	}

	return roomsNameRet;
}

Room* ModuleWorldManager::GetRoom(std::string roomName) const
{
	for (auto const& it : roomsInWorldList) {

		if (roomName == it->GetName().c_str()) {
			return it;
		}
	}

	FLY_ERROR("Room with name %s in ModuleWorldManager::GetRoom() could not be found", roomName.c_str());
	return nullptr;
}

Room* ModuleWorldManager::GetRoom(UID roomID) const
{
	for (auto const& it : roomsInWorldList) 
	{
		if (roomID == it->GetUID()) 	
			return it;		
	}

	FLY_ERROR("Room with ID %f in ModuleWorldManager::GetRoom() could not be found", roomID);
	return nullptr;
}

void ModuleWorldManager::SetStartRoom(Room* newFirstRoom)
{
	for (auto& currentRoom : roomsInWorldList)
	{
		if (currentRoom == newFirstRoom)
		{
			currentRoom->isStart = true; 
		}
		else
			currentRoom->isStart = false; 
	}
}

Room* ModuleWorldManager::GetStartRoom()
{
	for (auto& currentRoom : roomsInWorldList)
	{
		if (currentRoom->isStart)
		{
			return currentRoom;
		}
	}

	return nullptr; 
}

Room* ModuleWorldManager::GetFirstRoom()
{
	return roomsInWorldList.front();
}

int ModuleWorldManager::GetRoomsAmount() const
{
	return roomsInWoldAmount;
}

void ModuleWorldManager::SetSelectedRoom(Room* nextSelectedRoom, bool sendEvent)
{
	if (nextSelectedRoom != nullptr && nextSelectedRoom != selectedRoom)
	{
		//Room* selectedRoom = App->moduleRoomManager->GetSelectedRoom();
		//if(ViewportManager::getInstance()->viewportTexture != nullptr)
		//	selectedRoom->roomTextureID = ViewportManager::getInstance()->viewportTexture->GetTextureID();

		//flog("Set Selected Room: %s", nextSelectedRoom->GetName().c_str());

		if (selectedRoom != nullptr)
			selectedRoom->isSelected = false; 

		selectedRoom = nextSelectedRoom;
		selectedRoom->isSelected = true; 

		if(sendEvent)
			App->BroadCastEvent(FlyEngineEvent::ENTER_ROOM);

		if (App->flySection == FLY_SECTION_ROOM_EDIT)
		{
			selectedRoom->FitObjectUtils();
			selectedRoom->roomUIHandler->FitUIElements(); 
		}
		//NodeGraph::getInstance()->SelectNode(nextSelectedRoom->GetName());
	}
}

void ModuleWorldManager::SetSelectedRoom(UID selectedRoomUID, bool sendEvent)
{
	SetSelectedRoom(GetRoom(selectedRoomUID), sendEvent);
}

FlyObject* ModuleWorldManager::PrintInventoryObjectSelectionPopup()
{
	if (ImGui::BeginPopup("inventory_item_selection_popup"))
	{
		ImGui::Spacing();

		// Search Bar ---------------
		static char searchImageBuffer[256];
		ImGui::InputTextWithHint("##SearchToolInvItem", "Search...", searchImageBuffer, IM_ARRAYSIZE(searchImageBuffer));
		ImGui::SameLine();

		Texture* filterIcon = (Texture*)ResourceManager::getInstance()->GetResource("FilterIcon");
		ImGui::Image((ImTextureID)filterIcon->GetTextureID(), ImVec2(22, 22));

		ImGui::Spacing();

		ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0.12f, 0.14f, 0.17f, 1.00f));
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(2.0f, 2.0f));
		ImGui::BeginChild("##4ShowImage", ImVec2(ImGui::GetContentRegionAvailWidth(), 150));

		for (auto& currentRoom : roomsInWorldList)
		{
			for (auto& currentObject : currentRoom->objectsInRoom)
			{
				if (currentObject->flyObjectType != FlyObjectType::INVENTORY_ITEM)
					continue; 

				if (ImGui::Selectable(currentObject->GetName().c_str(), false, 0, ImVec2(ImGui::GetContentRegionAvail().x, 20)))
				{
					ImGui::EndChild();
					ImGui::PopStyleColor();
					ImGui::PopStyleVar();
					ImGui::CloseCurrentPopup();
					ImGui::EndPopup();
					return currentObject;
				}
			}

	
		}
		ImGui::EndChild();
		ImGui::PopStyleColor();
		ImGui::PopStyleVar();
		ImGui::EndPopup();
	}

	return nullptr;
}

void ModuleWorldManager::SetSelectedRoom(std::string roomName, bool sendEvent)
{
	SetSelectedRoom(GetRoom(roomName), sendEvent);
}

Room* ModuleWorldManager::GetSelectedRoom() const
{
	return selectedRoom;
}

RoomUIHandler* ModuleWorldManager::GetSelectedRoomUI() const
{
	if(selectedRoom != nullptr)
		return selectedRoom->roomUIHandler;

	return nullptr;
}
