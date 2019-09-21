#include "Room.h"
#include "Globals.h"
#include "NodeGraph.h"
#include "RandomNumberGenerator.h"
#include "Application.h"
#include "ModuleWorldManager.h"

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
	if (GetOutputConnectionsAmount() > 0) {
		for (auto it : outConnections) {
			delete it; 
		}

		outConnections.clear();
	}
}

RoomConnection* Room::ConnectToRoom(Room* destinationRoom)
{
	// Logic
	RoomConnection* newConnection = new RoomConnection(this, destinationRoom, "TestLink", false);
	outConnections.push_back(newConnection);
	destinationRoom->inRoomUIDs.push_back(GetRoomID());

	// Update Graph 
	NodeGraph::getInstance()->ConnectNodes(GetName(), "Out", destinationRoom->GetName(), "In", newConnection->connectionID);
	FLY_LOG("Room %s connected the LOGIC succesfuly with %s", roomName.c_str(), destinationRoom->GetName().c_str()); 
	App->moduleWorldManager->worldConnectionsAmount++; 
	return newConnection;
}

void Room::DeleteAllConnections()
{
	DeleteOutputConnections();
	DeleteInputConnections(); 
}

// Output Connections
void Room::DeleteOutputConnections()
{
	if (outConnections.size() <= 0)
		return; 

	for (auto it : outConnections) {
		it->destinationRoom->DeleteFromInRoomUIDs(roomID);
		it->DeleteOnGraph();

		delete it;
	}

	outConnections.clear(); 
}
void Room::DeleteOutputConnection(UID connectionToDelUID)
{
	for (auto it = outConnections.begin(); it != outConnections.end();) {
		
		if ((*it)->connectionID == connectionToDelUID) {

			DeleteFromInRoomUIDs(this->roomID);
			(*it)->DeleteOnGraph();

			delete (*it);
			it = outConnections.erase(it);

			break; 
		}
		else
			it++; 
	}
}

// Input connections
void Room::DeleteInputConnections()
{
	if (inRoomUIDs.size() <= 0)
		return;

	// Logic 
	for (auto it = inRoomUIDs.begin(); it != inRoomUIDs.end();) {

		DeleteInputConnection((*it)); 
	}

	inRoomUIDs.clear();	
}
void Room::DeleteInputConnection(UID roomToDelUID)
{
	Room* originRoom = App->moduleWorldManager->GetRoom(roomToDelUID);
	originRoom->DeleteOutputConnection(this->roomID); 
}
void Room::DeleteFromInRoomUIDs(UID roomToDelUID)
{
	for (auto it = inRoomUIDs.begin(); it != inRoomUIDs.end();) {
		if ((*it) == roomToDelUID)
			it = inRoomUIDs.erase(it);
		else
			it++;
	}
}

int Room::GetOutputConnectionsAmount() const
{
	return outConnections.size();
}

int Room::GetInputConnectionsAmount() const
{
	return inRoomUIDs.size();
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

void RoomConnection::DeleteOnGraph()
{
	NodeGraph::getInstance()->DeleteConnection(connectionID); 
	originRoom = nullptr; 
	destinationRoom = nullptr; 
}
