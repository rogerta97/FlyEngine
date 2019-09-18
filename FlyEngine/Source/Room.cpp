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
	for (auto it = roomLinks.begin(); it != roomLinks.end(); it++) {
		delete *it; 
	}

	roomLinks.clear(); 
}

UID Room::ConnectToRoom(Room* destinationRoom)
{
	RoomConnection* newConnection = new RoomConnection(destinationRoom, "TestLink", false);
	roomLinks.push_back(newConnection);

	FLY_LOG("Room %s connected the LOGIC succesfuly with %s", roomName.c_str(), destinationRoom->GetName().c_str()); 
	return newConnection->connectionID; 
}

string Room::GetName() const
{
	return roomName;
}

void Room::SetName(string newName)
{
	roomName = newName; 
}

RoomConnection::RoomConnection(Room* _roomConnected, string _connectionName, bool _isBidirectional)
{
	roomConnected = _roomConnected;
	connectionName = _connectionName;
	isBidirectional = _isBidirectional;
	connectionID = RandomNumberGenerator::getInstance()->GenerateUID();
}
