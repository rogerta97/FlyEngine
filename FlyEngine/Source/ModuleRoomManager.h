#ifndef  _MODULEENGINEMANAGER_H_
#define _MODULEENGINEMANAGER_H_

#include "Module.h"
#include "Globals.h"

#include <list>
#include <string>

using namespace std; 

class Room; 
class RoomConnection; 
class ModuleRoomManager : public Module
{
public:
	ModuleRoomManager(bool start_enabled = true);
	~ModuleRoomManager();

	// Module
	bool Start();
	update_status PreUpdate(float dt);
	update_status Update(float dt); 
	update_status PostUpdate(float dt);
	bool CleanUp();

	void ReceiveEvent(FlyEngineEvent eventType);
	void LoadRoomsData(); 

	// Rooms
	Room* CreateEmptyRoom(string roomName = "New Room"); 
	string* GetRoomsAsCombo(bool includeSelected = false); 
	void DeleteRoom(string roomName); 
	void DeleteRoom(UID roomID); 
	void DeleteRoom(Room* roomToDelete); 
	void CleanUpRooms();

	void SerializeRoomListNames(); 

	// Utility
	Room* GetRoom(string roomName) const;
	Room* GetRoom(UID roomID) const; 
	int GetRoomsAmount() const;

	void SetSelectedRoom(Room* selectedRoom); 
	void SetSelectedRoom(std::string roomName); 
	void SetSelectedRoom(UID roomName); 

	Room* GetSelectedRoom() const;

	int connectionsInWorldAmount;
	int roomsInWoldAmount;

public:
	list<Room*> roomsInWorldList;
	list<RoomConnection*> connectionsInWorldList; 

private: 
	Room* selectedRoom;

};


#endif // ! _MODULEENGINEMANAGER_H_

