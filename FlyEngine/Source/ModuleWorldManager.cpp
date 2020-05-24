#include "ModuleWorldManager.h"
#include "ModuleInput.h"
#include "Application.h"
#include "ModuleImGui.h"
#include "Blackboard.h"
#include "RandomNumberGenerator.h"
#include "GamePropertiesDockPanel.h"
#include "ViewportManager.h"
#include "GameInventory.h"
#include "AudioImporter.h"
#include "Room.h"
#include "NodeGraph.h"
#include "FontImporter.h"
#include "ImageImporter.h"
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
	return true;
}

update_status ModuleWorldManager::PreUpdate(float dt)
{
	return UPDATE_CONTINUE;
}

update_status ModuleWorldManager::Update(float dt)
{
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
	}

	return UPDATE_CONTINUE;
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

	static bool canDrag = false; 
	static int slotClicked = -1; 
	if (ImGui::IsMouseClicked(0))
	{
		canDrag = true;
	
		slotClicked = GetSlotClickedID();
		flog("Slot Clicked: %d", slotClicked);
		flog("CAN DRAG");
	}

	if (ImGui::IsMouseReleased(0))
	{
		canDrag = false;
		slotClicked = -1;
		flog("CAN NOT DRAG");
		
	}

	if (canDrag && slotClicked != -1 && ImGui::IsMouseDragPastThreshold(0, 15.0f))
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
		
		if (App->flySection == FlyEngineSection::FLY_SECTION_ROOM_EDIT && GetSelectedRoom() != nullptr){
			ViewportManager::getInstance()->drawGizmos = false; 
			GameInventory::getInstance()->CloseInventory();
		}

		break; 

	case FlyEngineEvent::ENGINE_STOP:

		if (App->flySection == FlyEngineSection::FLY_SECTION_ROOM_EDIT && GetSelectedRoom() != nullptr)
		{
			// For now they will be set to true, in the future they are going to be loaded from the viewportConfig.json
			//ViewportManager::getInstance()->drawClickableArea = true;
			ViewportManager::getInstance()->drawGizmos = true;
			GameInventory::getInstance()->ClearItems();
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

	// Load Global Blackboard 
	App->moduleWorldManager->globalBlackboard->LoadData("Global"); 

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
	if (nextSelectedRoom != nullptr) 
	{
		//Room* selectedRoom = App->moduleRoomManager->GetSelectedRoom();
		//if(ViewportManager::getInstance()->viewportTexture != nullptr)
		//	selectedRoom->roomTextureID = ViewportManager::getInstance()->viewportTexture->GetTextureID();

		if (selectedRoom != nullptr)
			selectedRoom->isSelected = false; 

		selectedRoom = nextSelectedRoom;
		selectedRoom->isSelected = true; 

		if(sendEvent)
			App->BroadCastEvent(FlyEngineEvent::ENTER_ROOM);
		//NodeGraph::getInstance()->SelectNode(nextSelectedRoom->GetName());
	}
}

void ModuleWorldManager::SetSelectedRoom(UID selectedRoomUID, bool sendEvent)
{
	SetSelectedRoom(GetRoom(selectedRoomUID), sendEvent);
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
