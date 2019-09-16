#include "ModuleWorldManager.h"
#include "Room.h"
#include "NodeGraph.h"
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
	// Create fake rooms 
	Room* roomToAdd = new Room("Forest"); 
	roomsInWorldList.push_back(roomToAdd);

	roomToAdd = new Room("Forest Room 1");
	roomsInWorldList.push_back(roomToAdd);

	//ConnectRooms(GetRoom("Forest"), GetRoom("Forest Room 1")); 

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

void ModuleWorldManager::CleanUpRooms()
{
	for (auto it = roomsInWorldList.begin(); it != roomsInWorldList.end(); it++) {
		(*it)->CleanUp();
		delete (*it);
	}

	roomsInWorldList.clear();
}

void ModuleWorldManager::ConnectRooms(Room* originRoom, Room* destinationRoom)
{
	originRoom->ConnectToRoom(destinationRoom);
	NodeGraph::getInstance()->ConnectNodes(originRoom->GetName(), "Out", destinationRoom->GetName(), "In");
}

Room* ModuleWorldManager::GetRoom(string roomName) const
{
	for (auto const& it : roomsInWorldList) {		

		if (roomName == it->GetName()) {
			return it;
		}		
	}

	FLY_ERROR("Room with name %s in ModuleWorldManager::GetRoom() could not be found", roomName.c_str()); 
	return nullptr;
}
