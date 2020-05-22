#ifndef  _MODULEENGINEMANAGER_H_
#define _MODULEENGINEMANAGER_H_

#include "Module.h"
#include "Globals.h"

#include <list>
#include <string>

using namespace std; 

class Room; 
class RoomConnection; 
class RoomUIHandler; 
class Blackboard;
class ModuleWorldManager : public Module
{
public:
	ModuleWorldManager(bool start_enabled = true);
	~ModuleWorldManager();

	// Module
	bool Start();
	update_status PreUpdate(float dt);
	update_status Update(float dt); 
	update_status PostUpdate(float dt);
	bool CleanUp();

	void SaveConfigData(); 

	void FitObjectUtils(); 
	void DeleteSingletones();

	void ReceiveEvent(FlyEngineEvent eventType);
	bool LoadAllData(); 

	// Rooms ------------
	Room* CreateEmptyRoom(string roomName = "New Room"); 
	const char** GetRoomsAsCombo(bool includeSelected = false); 
	void DeleteRoom(string roomName); 
	void DeleteRoom(UID roomID); 
	void DeleteRoom(Room* roomToDelete); 
	void CleanUpRooms();

	list<Room*>& GetRoomsInWorldList();

	void SerializeRoomListNames(); 

	// Utility ----------
	Room* GetRoom(string roomName) const;
	Room* GetRoom(UID roomID) const;
	Room* GetFirstRoom(); 
	int GetRoomsAmount() const;

	void SetStartRoom(Room* firstRoom); 
	Room* GetStartRoom(); 

	void SetSelectedRoom(Room* selectedRoom, bool sendEvent = false); 
	void SetSelectedRoom(std::string roomName, bool sendEvent = false);
	void SetSelectedRoom(UID roomName, bool sendEvent = false);

	Room* GetSelectedRoom() const;
	RoomUIHandler* GetSelectedRoomUI() const; 

	int connectionsInWorldAmount;
	int roomsInWoldAmount;

public:
	list<Room*> roomsInWorldList;
	list<RoomConnection*> connectionsInWorldList; 
	Blackboard* globalBlackboard; 

private: 
	Room* selectedRoom;

};


#endif // ! _MODULEENGINEMANAGER_H_

