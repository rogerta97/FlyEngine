#include "Room.h"
#include "mmgr.h"

Room::Room(string roomName)
{
	this->roomName = roomName; 
}

Room::~Room()
{
}

void Room::CleanUp()
{
	for (auto it = roomLinks.begin(); it != roomLinks.end(); it++) {
		(*it)->connectionName = nullptr; 
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
