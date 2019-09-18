#ifndef _ROOM_H_
#define _ROOM_H_


#include "RoomLink.h"
#include "Globals.h"
#include <list>
#include <string>

using namespace std; 

class Room; 
struct RoomConnection {

	RoomConnection(Room* _roomConnected, string _connectionName, bool _isBidirectional);

	Room* roomConnected;
	bool isBidirectional; 
	string connectionName;
	UID connectionID; 
};

class Room {

public:

	Room(string roomName); 
	~Room(); 

	void CleanUp(); 

	// Utility
	UID ConnectToRoom(Room* destinationRoom);

	// Encapsulation
	string GetName() const; 
	void SetName(string newName); 

private: 

	string roomName;
	list<RoomConnection*> roomLinks; 
};



#endif // !_ROOM_H_

