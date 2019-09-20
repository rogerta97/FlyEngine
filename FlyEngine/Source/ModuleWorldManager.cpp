#include "ModuleWorldManager.h"
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
	connectionsAmount = 0; 
	roomsAmount = 0;
}

ModuleWorldManager::~ModuleWorldManager()
{
}

bool ModuleWorldManager::Start()
{
	CreateEmptyRoom("Forest");
	CreateEmptyRoom("Forest Room 1");
	//CreateEmptyRoom("Forest Room 2");

	return true;
}

update_status ModuleWorldManager::PreUpdate(float dt)
{
	return UPDATE_CONTINUE;
}

update_status ModuleWorldManager::PostUpdate(float dt)
{
	FLY_WARNING("I HAVE %d CONNECTIONS", connectionsAmount);
	FLY_WARNING("I HAVE %d ROOMS", roomsAmount);

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

void ModuleWorldManager::CreateEmptyRoom(string roomName)
{
	roomsInWorldList.push_back(new Room(roomName));
	roomsAmount++;
	
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

			NodeGraph::getInstance()->DeleteAllConnections();
			(*it)->CleanUp();

			NodeGraph::getInstance()->DeleteNode((*it)->GetName());
			delete (*it);

			roomsInWorldList.erase(it);
			break;
		}
	}

	roomsAmount--; 
}

void ModuleWorldManager::DeleteRoom(UID roomID)
{
	for (auto it = roomsInWorldList.begin(); it != roomsInWorldList.end(); it++) {

		if ((*it)->GetRoomID() == roomID) {

			NodeGraph::getInstance()->DeleteAllConnections(); 
			(*it)->CleanUp(); 

			NodeGraph::getInstance()->DeleteNode((*it)->GetName());
			delete (*it);

			roomsInWorldList.erase(it);
			break;
		}
	}

	roomsAmount--;
}

void ModuleWorldManager::CleanUpRooms()
{
	for (auto it = roomsInWorldList.begin(); it != roomsInWorldList.end(); it++) {
		(*it)->CleanUp();
		delete (*it);
	}

	roomsInWorldList.clear();
	roomsAmount = 0; 
}

int ModuleWorldManager::GetConnectionsAmount() const
{
	return connectionsAmount; 
}

void ModuleWorldManager::ConnectRooms(UID originRoomID, UID destinationRoomID)
{
	ConnectRooms(GetRoomByID(originRoomID), GetRoomByID(destinationRoomID));
}

void ModuleWorldManager::ConnectRooms(Room* originRoom, Room* destinationRoom)
{
	// Logic
	RoomConnection* connection = originRoom->ConnectToRoom(destinationRoom); 

	// Graph
	NodeGraph::getInstance()->ConnectNodes(originRoom->GetName(), "Out", destinationRoom->GetName(), "In", connection->connectionID);	
	connectionsAmount++; 

	
}

void ModuleWorldManager::UnconnectRooms(Room* originRoom, Room* destinationRoom)
{
	UID deletedConnectionID = -1;

	// Logic
	for (auto it : roomsInWorldList) {
		if (it == originRoom) {
			deletedConnectionID = it->DeleteConnectionByID(destinationRoom); 
		}
	}

	// Graph 
	if (deletedConnectionID != -1) {
		NodeGraph::getInstance()->DeleteConnection(deletedConnectionID); 
	}
	else {
		FLY_ERROR("Connection to delete could not be found"); 
	}

	connectionsAmount--; 
}

void ModuleWorldManager::UnconnectRooms(std::string originRoomName, std::string destinationRoomName)
{
	UnconnectRooms(GetRoomByName(originRoomName), GetRoomByName(destinationRoomName)); 
}

void ModuleWorldManager::DeleteConnection(UID connectionID)
{
	// Logic
	for (auto it : roomsInWorldList) {		
		if (it->DeleteConnectionByID(connectionID)) {
			break;
		}
	}

	// Graph
	NodeGraph::getInstance()->DeleteConnection(connectionID); 
	connectionsAmount--; 
}

void ModuleWorldManager::DeleteConnectionsFromRoom(UID targetRoomID)
{
	// Logic 
	Room* targetRoom = GetRoomByID(targetRoomID); 
	vector<UID> connectionsCementery = targetRoom->DeleteAllConnections(); 

	// Graph 
	NodeGraph::DeleteConnections(connectionsCementery);
	connectionsAmount -= connectionsCementery.size();
}

Room* ModuleWorldManager::GetRoomByName(std::string roomName) const
{
	for (auto const& it : roomsInWorldList) {		

		if (roomName == it->GetName()) {
			return it;
		}		
	}

	FLY_ERROR("Room with name %s in ModuleWorldManager::GetRoom() could not be found", roomName.c_str()); 
	return nullptr;
}

Room* ModuleWorldManager::GetRoomByID(UID roomID) const
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
	return roomsAmount;
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
