#ifndef _ROOM_H_
#define _ROOM_H_

#include "RoomLink.h"
#include <list>
#include <string>

using namespace std; 

class Room; 
struct RoomLink {

	explicit RoomLink(Room* _roomConnected, string _connectionName, bool _isBidirectional) {
		roomConnected = _roomConnected;
		connectionName = _connectionName;
		isBidirectional = _isBidirectional; 
	}

	Room* roomConnected;
	bool isBidirectional; 
	string connectionName;
};

class Room {

public:

	Room(string roomName); 
	~Room(); 

	void CleanUp(); 

	// Utility
	void ConnectToRoom(Room* destinationRoom);

	// Encapsulation
	string GetName() const; 
	void SetName(string newName); 

private: 

	string roomName;
	list<RoomLink*> roomLinks; 
};



#endif // !_ROOM_H_

