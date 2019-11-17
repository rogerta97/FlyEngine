#ifndef _ROOM_H_
#define _ROOM_H_

#include "Globals.h"
#include <list>
#include <vector>
#include <string>

using namespace std; 

class Room; 
struct RoomConnection {

	RoomConnection(Room* _originRoom, Room* _roomConnected, string _connectionName, bool _isBidirectional);
	void DeleteOnGraph(); 

	Room* originRoom; 
	Room* destinationRoom;

	bool isBidirectional; 
	string connectionName;
	UID connectionID; 
};

class FlyObject; 
class Room {

public:

	Room(string roomName); 
	~Room(); 

	void CleanUp(); 

	// Connections --------------------------------------------------------------------
	RoomConnection* ConnectToRoom(Room* destinationRoom);
	RoomConnection* GetConnectionToRoom(UID dstRoomUID) const;

	void DeleteAllConnections();
	int GetTotalConnectionsAmount() const;
	const char* GetOutConnectionsAsCombo();

	// Output Connections ---------
	void BreakOutputConnections();
	void BreakOutputConnection(UID connectionUID);
	void BreakOutputConnection(Room* connectionUID);

	// Enter Connections ----------
	void BreakEnterConnections(); 
	void BreakEnterConnection(UID roomToDelUID);
	void BreakEnterConnectionFromInRoomUIDs(UID roomToDelUID);

	int GetEnterConnectionsAmount() const;
	int GetOutputConnectionsAmount() const;

	// Draw ---------------------------------------------------------------------------
	void DrawRoomObjects(); 

	// Factory ------------------------------------------------------------------------
	FlyObject* CreateFlyObject(std::string objectName); 
	void AddFlyObject(FlyObject* newFlyObject);
	void DeleteFlyObject(FlyObject* objectToDelete);

	// Encapsulation ------------------------------------------------------------------
	string GetName() const; 
	void SetName(string newName); 

	UID GetRoomID() const;
	void SetRoomID(UID newName);

	void SetSelectedObject(FlyObject* newObject); 
	FlyObject* GetSelectedObject() const;

	// Vars ---------------------------------------------------------------------------
	// Connections ---------
	list<RoomConnection*> outConnections;
	list<UID> inRoomUIDs;

	// Objects ---------
	list<FlyObject*> objectsInRoom;

private: 
	FlyObject* selectedObject; 
	UID roomID; 
	string roomName;
};



#endif // !_ROOM_H_

