#ifndef _ROOM_H_
#define _ROOM_H_


#include "RoomLink.h"
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

class Room {

public:

	Room(string roomName); 
	~Room(); 

	void CleanUp(); 

	// Connections 
	RoomConnection* ConnectToRoom(Room* destinationRoom);

	void BreakOutputConnections();
	void BreakOutputConnection(UID connectionUID); 
	void BreakOutputConnection(Room* connectionUID); 

	void BreakInputConnections(); 
	void BreakInputConnection(UID roomToDelUID);
	void BreakFromInRoomUIDs(UID roomToDelUID);

	void DeleteAllConnections(); 

	// Utility
	int GetOutputConnectionsAmount() const; 
	int GetInputConnectionsAmount() const; 
	RoomConnection* GetConnectionToRoom(UID dstRoomUID) const;
	int GetTotalConnectionsAmount() const; 

	// Encapsulation
	string GetName() const; 
	void SetName(string newName); 

	UID GetRoomID() const;
	void SetRoomID(UID newName);

private: 

	UID roomID; 
	string roomName;
	list<RoomConnection*> outConnections;
	list<UID> inRoomUIDs; 
};



#endif // !_ROOM_H_

