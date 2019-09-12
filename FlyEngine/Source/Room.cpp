#include "Room.h"

Room::Room(string roomName)
{
	this->roomName = roomName; 
}

Room::~Room()
{
}

string Room::GetName() const
{
	return roomName;
}

void Room::SetName(string newName)
{
	roomName = newName; 
}
