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
	App->moduleWorldManager->connectionsInWorldAmount++; 
	return newConnection;
}

void Room::DeleteAllConnections()
{
	BreakOutputConnections();
	BreakInputConnections(); 
}

// Output Connections
void Room::BreakOutputConnections()
{
	if (outConnections.size() <= 0)
		return; 

	for (auto it : outConnections) {
		it->destinationRoom->BreakFromInRoomUIDs(roomID);
		it->DeleteOnGraph();

		delete it;
	}

	App->moduleWorldManager->connectionsInWorldAmount -= outConnections.size(); 
	outConnections.clear(); 
}

RoomConnection* Room::GetConnectionToRoom(UID dstRoomUID) const
{
	for (auto it : outConnections) {		
		if (it->destinationRoom->roomID == dstRoomUID) {
			return it; 
		}
	}
}

int Room::GetTotalConnectionsAmount() const
{
	return GetOutputConnectionsAmount() + GetInputConnectionsAmount(); 
}

const char* Room::GetOutConnectionsAsCombo()
{
	string returnStr;
	for (auto& it : outConnections) {
		returnStr += (it)->connectionName; 
		returnStr += '\0';
	}

	return returnStr.c_str(); 
}


void Room::BreakOutputConnection(UID connectionToDelUID)
{
	for (auto it = outConnections.begin(); it != outConnections.end();) {
		
		if ((*it)->connectionID == connectionToDelUID) {

			(*it)->DeleteOnGraph();

			delete (*it);
			it = outConnections.erase(it);
			App->moduleWorldManager->connectionsInWorldAmount -= 1; 
			break; 
		}
		else
			it++; 
	}
}
void Room::BreakOutputConnection(Room* targetRoomConnected) 
{
	RoomConnection* roomConnection = GetConnectionToRoom(targetRoomConnected->GetRoomID()); 
	BreakOutputConnection(roomConnection->connectionID); 
}
// Input connections
void Room::BreakInputConnections()
{
	if (inRoomUIDs.size() <= 0)
		return;

	for (auto it = inRoomUIDs.begin(); it != inRoomUIDs.end();) {
		BreakInputConnection((*it)); 
		it = inRoomUIDs.erase(it);
	}

	inRoomUIDs.clear();	
}
void Room::BreakInputConnection(UID roomToDelUID)
{
	Room* originRoom = App->moduleWorldManager->GetRoom(roomToDelUID);
	originRoom->BreakOutputConnection(this); 
}
void Room::BreakFromInRoomUIDs(UID roomToDelUID)
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
