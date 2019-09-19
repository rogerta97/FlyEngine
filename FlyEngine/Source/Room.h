#ifndef _ROOM_H_
#define _ROOM_H_


#include "RoomLink.h"
#include "Globals.h"
#include <list>
#include <string>

using namespace std; 

class Room; 
struct RoomConnection {

	RoomConnection(Room* _originRoom, Room* _roomConnected, string _connectionName, bool _isBidirectional);

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
	UID DeleteConnectionByID(Room* destinationRoom); 
	bool DeleteConnectionByID(UID destinationRoomID); 

	list<RoomConnection*> GetConnectionsList() const;
	int GetConnectionsAmount() const; 

	// Encapsulation
	string GetName() const; 
	void SetName(string newName); 

private: 
	string roomName;
	list<RoomConnection*> roomConnections; 
};



#endif // !_ROOM_H_

