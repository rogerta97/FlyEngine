#include "Room.h"
#include "Globals.h"
#include "NodeGraph.h"
#include "RandomNumberGenerator.h"

Room::Room(string roomName)
{
	this->roomName = roomName;

	// Add The Room to the NodeGraph 
	static int placer = 50;
	NodeGraph::getInstance()->CreateNode(roomName, ImVec2(placer, 50));
	
	placer += 250; 
}

Room::~Room()
{
}

void Room::CleanUp()
{
	for (auto it = roomConnections.begin(); it != roomConnections.end(); it++) {
		delete *it; 
	}

	roomConnections.clear(); 
}

RoomConnection* Room::ConnectToRoom(Room* destinationRoom)
{
	RoomConnection* newConnection = new RoomConnection(this, destinationRoom, "TestLink", false);
	roomConnections.push_back(newConnection);

	FLY_LOG("Room %s connected the LOGIC succesfuly with %s", roomName.c_str(), destinationRoom->GetName().c_str()); 
	return newConnection;
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

RoomConnection::RoomConnection(Room* _originRoom, Room* _roomConnected, string _connectionName, bool _isBidirectional)
{
	originRoom = _originRoom;
	destinationRoom = _roomConnected;
	connectionName = _connectionName;
	isBidirectional = _isBidirectional;
	connectionID = RandomNumberGenerator::getInstance()->GenerateUID();
}
