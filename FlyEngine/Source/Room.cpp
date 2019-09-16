#include "Room.h"
#include "Globals.h"
#include "NodeGraph.h"

Room::Room(string roomName)
{
	this->roomName = roomName;
	FLY_LOG("Room %s created", roomName.c_str()); 

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

void Room::ConnectToRoom(Room* destinationRoom)
{
	RoomLink* newLink = new RoomLink(destinationRoom, "TestLink", false);
	roomLinks.push_back(newLink);

	FLY_LOG("Room %s connected the logic succesfuly with %s", roomName.c_str(), destinationRoom->GetName().c_str()); 
}

string Room::GetName() const
{
	return roomName;
}

void Room::SetName(string newName)
{
	roomName = newName; 
}
