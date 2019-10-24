#include "ModuleRoomManager.h"
#include "ModuleInput.h"
#include "Application.h"
#include "ModuleImGui.h"
#include "RandomNumberGenerator.h"
#include "GraphPropertiesDockPanel.h"
#include "Room.h"
#include "NodeGraph.h"
#include "FileSystem.h"
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
	Room* forestRoom = CreateEmptyRoom("Forest");
	Room* Lake = CreateEmptyRoom("Lake");
	Room* Bridge = CreateEmptyRoom("Bridge");

	forestRoom->ConnectToRoom(Lake);
	forestRoom->ConnectToRoom(Bridge);
	Lake->ConnectToRoom(forestRoom); 

	SetSelectedRoom(forestRoom);
	App->moduleImGui->AddaptToFlySection(FLY_SECTION_ROOM_EDIT); 

	return true;
}

update_status ModuleRoomManager::PreUpdate(float dt)
{
	return UPDATE_CONTINUE;
}

update_status ModuleRoomManager::Update(float dt)
{
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
	NodeGraph::getInstance()->DeleteAllConnections();
	NodeGraph::getInstance()->DeleteAllNodes();

	FileSystem::getInstance()->Delete();
	RandomNumberGenerator::getInstance()->Delete();

	return true;
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
	UID roomUID = GetRoom(roomName)->GetRoomID();

	if (selectedRoom->GetRoomID() == roomUID) {
		selectedRoom = nullptr; 
	}

	DeleteRoom(roomUID);	
}

void ModuleRoomManager::DeleteRoom(UID roomID)
{
	for (auto it = roomsInWorldList.begin(); it != roomsInWorldList.end(); it++) {

		if ((*it)->GetRoomID() == roomID) {

			(*it)->DeleteAllConnections();

			NodeGraph::getInstance()->DeleteNode((*it)->GetName());
			delete (*it);

			roomsInWorldList.erase(it);
			break;
		}
	}

	roomsInWoldAmount--;
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

const char* ModuleRoomManager::GetRoomsAsCombo(bool includeSelected)
{
	string resultStr; 
	for (auto& it : roomsInWorldList) {

		if ((it) == selectedRoom && !includeSelected) {
			continue; 
		}

		resultStr += (it)->GetName() + '\0';
	}

	return resultStr.c_str(); 
}

Room* ModuleRoomManager::GetRoom(std::string roomName) const
{
	for (auto const& it : roomsInWorldList) {

		if (roomName == it->GetName()) {
			return it;
		}
	}

	FLY_ERROR("Room with name %s in ModuleRoomManager::GetRoom() could not be found", roomName.c_str());
	return nullptr;
}

Room* ModuleRoomManager::GetRoom(UID roomID) const
{
	for (auto const& it : roomsInWorldList) {

		if (roomID == it->GetRoomID()) {
			return it;
		}
	}

	FLY_ERROR("Room with ID %f in ModuleRoomManager::GetRoom() could not be found", roomID);
	return nullptr;
}

int ModuleRoomManager::GetRoomsAmount() const
{
	return roomsInWoldAmount;
}

void ModuleRoomManager::SetSelectedRoom(Room* nextSelectedRoom)
{
	if (nextSelectedRoom != nullptr) {
		selectedRoom = nextSelectedRoom;
		NodeGraph::getInstance()->SelectNode(nextSelectedRoom->GetName());
	}
}

void ModuleRoomManager::SetSelectedRoom(UID selectedRoomUID)
{
	SetSelectedRoom(GetRoom(selectedRoomUID)); 
}

void ModuleRoomManager::SetSelectedRoom(std::string roomName)
{
	SetSelectedRoom(GetRoom(roomName)); 
}

Room* ModuleRoomManager::GetSelectedRoom() const
{
	return selectedRoom;
}
