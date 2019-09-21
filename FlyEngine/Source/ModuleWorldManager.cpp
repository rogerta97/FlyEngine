#include "ModuleWorldManager.h"
#include "ModuleInput.h"
#include "Application.h"
#include "ModuleImGui.h"
#include "RandomNumberGenerator.h"
#include "GraphPropertiesDockPanel.h"
#include "Room.h"
#include "NodeGraph.h"
#include "FileSystem.h"
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
	/*Room* forestRoom = CreateEmptyRoom("Forest");
	Room* Lake = CreateEmptyRoom("Lake");
	Room* Bridge = CreateEmptyRoom("Bridge");

	forestRoom->ConnectToRoom(Lake);
	forestRoom->ConnectToRoom(Bridge);
	Lake->ConnectToRoom(forestRoom); 
	Bridge->ConnectToRoom(forestRoom);*/

	return true;
}

update_status ModuleWorldManager::PreUpdate(float dt)
{
	return UPDATE_CONTINUE;
}

update_status ModuleWorldManager::PostUpdate(float dt)
{

	if (App->moduleInput->GetKey(SDL_SCANCODE_S)) {

		FLY_WARNING("Rooms Count: %d", roomsInWoldAmount); 
		FLY_WARNING("Connections Count: %d", connectionsInWorldAmount); 
	}

	return UPDATE_CONTINUE;
}

bool ModuleWorldManager::CleanUp()
{
	CleanUpRooms();
	NodeGraph::getInstance()->DeleteAllConnections();
	NodeGraph::getInstance()->DeleteAllNodes();

	FileSystem::getInstance()->Delete();
	RandomNumberGenerator::getInstance()->Delete();

	return true;
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
	UID roomUID = GetRoom(roomName)->GetRoomID();
	DeleteRoom(roomUID);
}

void ModuleWorldManager::DeleteRoom(UID roomID)
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

void ModuleWorldManager::CleanUpRooms()
{
	for (auto it = roomsInWorldList.begin(); it != roomsInWorldList.end(); it++) {
		(*it)->CleanUp();
		delete (*it);
	}

	roomsInWorldList.clear();
	roomsInWoldAmount = 0;
}

Room* ModuleWorldManager::GetRoom(std::string roomName) const
{
	for (auto const& it : roomsInWorldList) {

		if (roomName == it->GetName()) {
			return it;
		}
	}

	FLY_ERROR("Room with name %s in ModuleWorldManager::GetRoom() could not be found", roomName.c_str());
	return nullptr;
}

Room* ModuleWorldManager::GetRoom(UID roomID) const
{
	for (auto const& it : roomsInWorldList) {

		if (roomID == it->GetRoomID()) {
			return it;
		}
	}

	FLY_ERROR("Room with ID %f in ModuleWorldManager::GetRoom() could not be found", roomID);
	return nullptr;
}

int ModuleWorldManager::GetRoomsAmount() const
{
	return roomsInWoldAmount;
}

void ModuleWorldManager::SetSelectedRoom(Room* selectedRoom)
{
	this->selectedRoom = selectedRoom;
}

void ModuleWorldManager::SetSelectedRoom(std::string roomName)
{
	Room* nextSelectedRoom = GetRoom(roomName);

	if (nextSelectedRoom != nullptr) {
		selectedRoom = nextSelectedRoom;
		NodeGraph::getInstance()->SelectNode(roomName);
	}
}

Room* ModuleWorldManager::GetSelectedRoom() const
{
	return selectedRoom;
}
