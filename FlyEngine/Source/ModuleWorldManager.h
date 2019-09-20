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

	void ConnectRooms(UID originRoomID, UID destinationRoomID); 
	void ConnectRooms(Room* originRoom, Room* destinationRoom);

	void DeleteRoom(string roomName); 
	void DeleteRoom(UID roomID); 

	void DeleteSelectedRoom(); 
	void CleanUpRooms();

	// Connections
	void UnconnectRooms(Room* originRoom, Room* destinationRoom); 
	void UnconnectRooms(std::string originRoomName, std::string destinationRoomName);
	void DeleteConnection(UID connectionID);
	void DeleteConnectionsFromRoom(UID targetRoom);
	int GetConnectionsAmount() const; 

	// Utility
	Room* GetRoomByName(string roomName) const;
	Room* GetRoomByID(UID roomID) const; 
	int GetRoomsAmount() const;

	void SetSelectedRoom(Room* selectedRoom); 
	void SetSelectedRoom(std::string roomName); 

	Room* GetSelectedRoom() const;

public:
	list<Room*> roomsInWorldList;
	list<RoomConnection*> connectionsInWorldList; 

private: 
	Room* selectedRoom;
	
	int connectionsAmount; 
	int roomsAmount; 
};


#endif // ! _MODULEENGINEMANAGER_H_

