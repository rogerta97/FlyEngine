#include "ModuleRoomManager.h"
#include "ModuleInput.h"
#include "Application.h"
#include "ModuleImGui.h"
#include "RandomNumberGenerator.h"
#include "GamePropertiesDockPanel.h"
#include "ViewportManager.h"
#include "GameInventory.h"
#include "AudioImporter.h"
#include "Room.h"
#include "NodeGraph.h"
#include "FontImporter.h"
#include "ImageImporter.h"
#include "ResourceManager.h"
#include "MyFileSystem.h"
#include "FlyObject.h"
#include "Texture.h"
#include "TextureMSAA.h"
#include "SaveAndLoad.h"

#include "mmgr.h"

ModuleRoomManager::ModuleRoomManager(bool start_enabled)
{
	moduleType = MODULE_ENGINE_MANAGER;
	connectionsInWorldAmount = 0;
	roomsInWoldAmount = 0;
}

ModuleRoomManager::~ModuleRoomManager()
{
}

bool ModuleRoomManager::Start()
{
	
	return true;
}

update_status ModuleRoomManager::PreUpdate(float dt)
{
	return UPDATE_CONTINUE;
}

update_status ModuleRoomManager::Update(float dt)
{
	for (auto& currentRoom : roomsInWorldList)
	{
		currentRoom->Update(); 
	}

	return UPDATE_CONTINUE;
}

update_status ModuleRoomManager::PostUpdate(float dt)
{

	if (App->moduleInput->GetKey(SDL_SCANCODE_S)) {

		FLY_WARNING("Rooms Count: %d", roomsInWoldAmount); 
		FLY_WARNING("Connections Count: %d", connectionsInWorldAmount); 
	}

	return UPDATE_CONTINUE;
}

bool ModuleRoomManager::CleanUp()
{
	CleanUpRooms();
	DeleteSingletones();

	return true;
}

void ModuleRoomManager::DeleteSingletones()
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

void ModuleRoomManager::ReceiveEvent(FlyEngineEvent eventType)
{
	switch (eventType)
	{
	case FlyEngineEvent::ENGINE_PLAY:
		
		if (App->flySection == FlyEngineSection::FLY_SECTION_ROOM_EDIT && GetSelectedRoom() != nullptr){
			ViewportManager::getInstance()->drawGizmos = false; 
		}

		break; 

	case FlyEngineEvent::ENGINE_STOP:

		if (App->flySection == FlyEngineSection::FLY_SECTION_ROOM_EDIT && GetSelectedRoom() != nullptr)
		{
			// For now they will be set to true, in the future they are going to be loaded from the viewportConfig.json
			//ViewportManager::getInstance()->drawClickableArea = true;
			ViewportManager::getInstance()->drawGizmos = true;
		}

		break; 

	case FlyEngineEvent::ENTER_ROOM:

		// Save Image For Thumbnail
	/*	if (selectedRoom != nullptr)
		{
			selectedRoom->roomThumbnail = ViewportManager::getInstance()->viewportTexture->GetTextureID(); 
		}*/

		// Play On Scene Enter Events
		if (App->flySection == FlyEngineSection::FLY_SECTION_ROOM_EDIT && App->isEngineInPlayMode && GetSelectedRoom() != nullptr)
		{
			for (auto& currentObject : GetSelectedRoom()->objectsInRoom)
			{
				currentObject->OnSceneEnter(); 
			}
		}

		break;
	}
}

bool ModuleRoomManager::LoadRoomsData()
{
	string roomsDirectory = MyFileSystem::getInstance()->GetSavedDataDirectory() + "RoomsData";
	vector<string> roomsSavedFiles;
	MyFileSystem::getInstance()->GetFilesInDirectory(roomsDirectory.c_str(), roomsSavedFiles, false);

	for (auto& currentRoomFile : roomsSavedFiles)
	{
		JSON_Value* root = json_parse_file(string(roomsDirectory + "\\" + currentRoomFile).c_str());
		JSON_Object* root_obj = json_value_get_object(root);

		string currentRoomName = json_object_dotget_string(root_obj, "RoomData.Name");
		Room* newRoom = App->moduleRoomManager->CreateEmptyRoom(currentRoomName);

		string uidStr = string("RoomData.UID");
		newRoom->SetUID(json_object_dotget_number(root_obj, uidStr.c_str()));
	}

	for (auto& currentRoom : roomsInWorldList)
	{
		SaveAndLoad::getInstance()->LoadDataToRoom(roomsDirectory + "\\" + currentRoom->GetName() + ".json", currentRoom);

		// Load Thumbnail 
		string thumbnailStr = MyFileSystem::getInstance()->GetThumbnilesDirectory() + "\\" + to_string((int)currentRoom->GetUID()) + "_Thumbnail";
		Texture* newThumbnail = ImageImporter::getInstance()->LoadTexture(thumbnailStr.c_str(), false);

		//if(newThumbnail != nullptr && ResourceManager::getInstance()->AddResource((Resource*)newThumbnail, to_string((int)currentRoom->GetUID()) + "_Thumbnail"))
		//	currentRoom->roomThumbnail = newThumbnail->GetTextureID(); 
	}


	
	if (roomsSavedFiles.size() > 0)
		return true; 

	return false; 
}

Room* ModuleRoomManager::CreateEmptyRoom(string roomName)
{
	Room* newRoom = new Room(roomName);
	roomsInWorldList.push_back(newRoom);
	roomsInWoldAmount++;
	return newRoom;
}

void ModuleRoomManager::DeleteRoom(string roomName)
{
	UID roomUID = GetRoom(roomName)->GetUID();

	if (selectedRoom->GetUID() == roomUID) {
		selectedRoom = nullptr; 
	}

	DeleteRoom(roomUID);	
}

void ModuleRoomManager::DeleteRoom(UID roomID)
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

void ModuleRoomManager::DeleteRoom(Room* roomToDelete)
{
	DeleteRoom(roomToDelete->GetUID()); 
}

void ModuleRoomManager::CleanUpRooms()
{
	for (auto it = roomsInWorldList.begin(); it != roomsInWorldList.end(); it++) {
		(*it)->CleanUp();
		delete (*it);
	}

	roomsInWorldList.clear();
	roomsInWoldAmount = 0;
}

list<Room*>& ModuleRoomManager::GetRoomsInWorldList()
{
	return roomsInWorldList; 
}

void ModuleRoomManager::SerializeRoomListNames()
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

const char** ModuleRoomManager::GetRoomsAsCombo(bool includeSelected)
{
	const char* roomsNameRet[256];
	
	int count = 0; 
	for (auto& it : roomsInWorldList) {
		roomsNameRet[count++] = it->GetName().c_str(); 
	}

	return roomsNameRet;
}

Room* ModuleRoomManager::GetRoom(std::string roomName) const
{
	for (auto const& it : roomsInWorldList) {

		if (roomName == it->GetName().c_str()) {
			return it;
		}
	}

	FLY_ERROR("Room with name %s in ModuleRoomManager::GetRoom() could not be found", roomName.c_str());
	return nullptr;
}

Room* ModuleRoomManager::GetRoom(UID roomID) const
{
	for (auto const& it : roomsInWorldList) 
	{
		if (roomID == it->GetUID()) 	
			return it;		
	}

	FLY_ERROR("Room with ID %f in ModuleRoomManager::GetRoom() could not be found", roomID);
	return nullptr;
}

void ModuleRoomManager::SetStartRoom(Room* newFirstRoom)
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

Room* ModuleRoomManager::GetStartRoom()
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

Room* ModuleRoomManager::GetFirstRoom()
{
	return roomsInWorldList.front();
}

int ModuleRoomManager::GetRoomsAmount() const
{
	return roomsInWoldAmount;
}

void ModuleRoomManager::SetSelectedRoom(Room* nextSelectedRoom, bool sendEvent)
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

void ModuleRoomManager::SetSelectedRoom(UID selectedRoomUID, bool sendEvent)
{
	SetSelectedRoom(GetRoom(selectedRoomUID), sendEvent);
}

void ModuleRoomManager::SetSelectedRoom(std::string roomName, bool sendEvent)
{
	SetSelectedRoom(GetRoom(roomName), sendEvent);
}

Room* ModuleRoomManager::GetSelectedRoom() const
{
	return selectedRoom;
}

RoomUIHandler* ModuleRoomManager::GetSelectedRoomUI() const
{
	if(selectedRoom != nullptr)
		return selectedRoom->roomUIHandler;

	return nullptr;
}
