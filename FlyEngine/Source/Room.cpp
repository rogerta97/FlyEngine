#include "Room.h"
#include "Globals.h"
#include "NodeGraph.h"
#include "ViewportManager.h"
#include "RandomNumberGenerator.h"
#include "Application.h"
#include "FlyObject.h"
#include "TextureMSAA.h"
#include "ModuleRoomManager.h"

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
	App->moduleRoomManager->connectionsInWorldAmount++; 
	return newConnection;
}

void Room::DeleteAllConnections()
{
	BreakOutputConnections();
	BreakEnterConnections(); 
}

// Output Connections
void Room::BreakOutputConnections()
{
	if (outConnections.size() <= 0)
		return; 

	for (auto it : outConnections) {
		it->destinationRoom->BreakEnterConnectionFromInRoomUIDs(roomID);
		it->DeleteOnGraph();

		delete it;
	}

	App->moduleRoomManager->connectionsInWorldAmount -= outConnections.size(); 
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
	return GetOutputConnectionsAmount() + GetEnterConnectionsAmount(); 
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
			App->moduleRoomManager->connectionsInWorldAmount -= 1; 
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
void Room::BreakEnterConnections()
{
	if (inRoomUIDs.size() <= 0)
		return;

	for (auto it = inRoomUIDs.begin(); it != inRoomUIDs.end();) {
		BreakEnterConnection((*it)); 
		it = inRoomUIDs.erase(it);
	}

	inRoomUIDs.clear();	
}
void Room::BreakEnterConnection(UID roomToDelUID)
{
	Room* originRoom = App->moduleRoomManager->GetRoom(roomToDelUID);
	originRoom->BreakOutputConnection(this); 
}
void Room::BreakEnterConnectionFromInRoomUIDs(UID roomToDelUID)
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

void Room::DrawRoomObjects()
{
	for (auto& it : objectsInRoom) {
		(it)->Draw(); 
	}

}

int Room::GetEnterConnectionsAmount() const
{
	return inRoomUIDs.size();
}

FlyObject* Room::CreateFlyObject(std::string objectName)
{
	FlyObject* newObject = new FlyObject(objectName); 
	objectsInRoom.push_back(newObject); 
	FLY_LOG("New Object Created"); 
	return newObject; 
}

void Room::DeleteFlyObject()
{
	for (auto& it : objectsInRoom) {
		delete(it); 
	}
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
