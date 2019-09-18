#ifndef  _MODULEENGINEMANAGER_H_
#define _MODULEENGINEMANAGER_H_

#include "Module.h"
#include "Globals.h"

#include <list>
#include <string>

using namespace std; 

class Room; 
class RoomConnection; 
class ModuleWorldManager : public Module
{
public:
	ModuleWorldManager(bool start_enabled = true);
	~ModuleWorldManager();

	// Module
	bool Start();
	update_status PreUpdate(float dt);
	update_status PostUpdate(float dt);
	bool CleanUp();

	// Rooms
	void CreateEmptyRoom(string roomName = "New Room"); 
	void ConnectRooms(Room* originRoom, Room* destinationRoom);

	void DeleteRoom(string roomName); 
	void CleanUpRooms();

	// Connections
	//std::list<RoomConnection*> GetConnectionsFromRoom(Room* originRoom);
	int GetConnectionsAmount() const; 

	// Utility
	Room* GetRoomByName(string roomName) const;
	int GetRoomsAmount() const;

	void SetSelectedRoom(Room* selectedRoom); 
	void SetSelectedRoom(std::string roomName); 

	Room* GetSelectedRoom() const;

public:
	list<Room*> roomsInWorldList;
	list<RoomConnection*> connectionsInWorldList; 

private: 
	Room* selectedRoom; 
};


#endif // ! _MODULEENGINEMANAGER_H_

