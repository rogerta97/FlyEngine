#include "Room.h"
#include "Globals.h"
#include "NodeGraph.h"
#include "RandomNumberGenerator.h"

Room::Room(string roomName)
{
	this->roomName = roomName;
	roomID = RandomNumberGenerator::GenerateUID(); 
	// Add The Room to the NodeGraph 
	static int placer = 50;
	NodeGraph::getInstance()->CreateNode(roomName, ImVec2(placer, 50), roomID);
	
	placer += 250; 
}

Room::~Room()
{
}

void Room::CleanUp()
{
	if (GetConnectionsAmount() > 0) {
		for (auto it : roomConnections) {
			delete it; 
		}

		roomConnections.clear();
	}
}

RoomConnection* Room::ConnectToRoom(Room* destinationRoom)
{
	RoomConnection* newConnection = new RoomConnection(this, destinationRoom, "TestLink", false);
	roomConnections.push_back(newConnection);

	FLY_LOG("Room %s connected the LOGIC succesfuly with %s", roomName.c_str(), destinationRoom->GetName().c_str()); 
	return newConnection;
}

bool Room::DeleteConnectionByID(UID connectionToBreak)
{
	for (auto it = roomConnections.begin(); it != roomConnections.end(); it++) {
		if ((*it)->connectionID == connectionToBreak) {
			UID returnUID = (*it)->connectionID;
			delete (*it);
			roomConnections.erase(it);
			
			return true;
		}
	}

	return false;
}

/// Returns UID of the connections deleted
vector<UID> Room::DeleteAllConnections()
{
	vector<UID> deletedConnectionsUID; 

	for (auto it = roomConnections.begin(); it != roomConnections.end();) {
		deletedConnectionsUID.push_back((*it)->connectionID); 
		delete (*it); 
		it = roomConnections.erase(it); 
	}

	return deletedConnectionsUID; 
}

UID Room::DeleteConnectionByID(Room* destinationRoom)
{
	for (auto it = roomConnections.begin(); it != roomConnections.end(); it++) {
		if ((*it)->destinationRoom == destinationRoom) {
			UID returnUID = (*it)->connectionID; 
			roomConnections.erase(it); 
			delete (*it); 
			return returnUID;
		}
	}

	return -1; 
}

list<RoomConnection*> Room::GetConnectionsList() const
{
	return roomConnections;
}

int Room::GetConnectionsAmount() const
{
	return roomConnections.size();
}

string Room::GetName() const
{
	return roomName;
}

void Room::SetName(string newName)
{
	roomName = newName; 
}

UID Room::GetRoomID() const
{
	return roomID; 
}

void Room::SetRoomID(UID roomID)
{
	roomID = roomID;
}

RoomConnection::RoomConnection(Room* _originRoom, Room* _roomConnected, string _connectionName, bool _isBidirectional)
{
	originRoom = _originRoom;
	destinationRoom = _roomConnected;
	connectionName = _connectionName;
	isBidirectional = _isBidirectional;
	connectionID = RandomNumberGenerator::getInstance()->GenerateUID();
}
