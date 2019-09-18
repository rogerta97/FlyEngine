#include "ModuleWorldManager.h"
#include "Application.h"
#include "ModuleImGui.h"
#include "GraphPropertiesDockPanel.h"
#include "Room.h"
#include "NodeGraph.h"
#include "FileSystem.h"
#include "mmgr.h"

ModuleWorldManager::ModuleWorldManager(bool start_enabled)
{
	moduleType = MODULE_ENGINE_MANAGER;
}

ModuleWorldManager::~ModuleWorldManager()
{
}

bool ModuleWorldManager::Start()
{
	//CreateEmptyRoom("Forest");
	//CreateEmptyRoom("Forest Room 1");
	//CreateEmptyRoom("Forest Room 2");

	return true;
}

update_status ModuleWorldManager::PreUpdate(float dt)
{
	return UPDATE_CONTINUE;
}

update_status ModuleWorldManager::PostUpdate(float dt)
{
	return UPDATE_CONTINUE;
}

bool ModuleWorldManager::CleanUp()
{
	CleanUpRooms();	
	NodeGraph::getInstance()->DeleteAllNodes(); 
	NodeGraph::getInstance()->DeleteAllConnections(); 

	return true;
}

void ModuleWorldManager::CreateEmptyRoom(string roomName)
{
	roomsInWorldList.push_back(new Room(roomName));
}

void ModuleWorldManager::DeleteSelectedRoom()
{
	if(selectedRoom != nullptr)
		DeleteRoom(selectedRoom->GetName()); 
}

void ModuleWorldManager::DeleteRoom(string roomName)
{
	for (auto it = roomsInWorldList.begin(); it != roomsInWorldList.end(); it++) {
		
		if ((*it)->GetName() == roomName) {
			
			(*it)->CleanUp();
			NodeGraph::getInstance()->DeleteNode((*it)->GetName()); 
			delete (*it);
			roomsInWorldList.erase(it);		
			break; 
		}	
	}
}

void ModuleWorldManager::CleanUpRooms()
{
	for (auto it = roomsInWorldList.begin(); it != roomsInWorldList.end(); it++) {
		(*it)->CleanUp();
		delete (*it);
	}

	roomsInWorldList.clear();
}

int ModuleWorldManager::GetConnectionsAmount() const
{
	return connectionsInWorldList.size();
}

void ModuleWorldManager::ConnectRooms(Room* originRoom, Room* destinationRoom)
{
	// Logic
	RoomConnection* connection = originRoom->ConnectToRoom(destinationRoom); 
	connectionsInWorldList.push_back(connection);

	// Graph
	NodeGraph::getInstance()->ConnectNodes(originRoom->GetName(), "Out", destinationRoom->GetName(), "In", connection->connectionID);	
}

Room* ModuleWorldManager::GetRoomByName(string roomName) const
{
	for (auto const& it : roomsInWorldList) {		

		if (roomName == it->GetName()) {
			return it;
		}		
	}

	FLY_ERROR("Room with name %s in ModuleWorldManager::GetRoom() could not be found", roomName.c_str()); 
	return nullptr;
}

int ModuleWorldManager::GetRoomsAmount() const
{
	return roomsInWorldList.size();
}

void ModuleWorldManager::SetSelectedRoom(Room* selectedRoom)
{
	this->selectedRoom = selectedRoom; 
}

void ModuleWorldManager::SetSelectedRoom(std::string roomName)
{
	Room* nextSelectedRoom = GetRoomByName(roomName);

	if (nextSelectedRoom != nullptr) {		
		selectedRoom = nextSelectedRoom;
		NodeGraph::getInstance()->SelectNode(roomName);
	}
}

Room* ModuleWorldManager::GetSelectedRoom() const
{
	return selectedRoom;
}
