#ifndef _ROOM_H_
#define _ROOM_H_

#include "RoomLink.h"
#include <list>
#include <string>

using namespace std; 

class Room; 
struct RoomLink {
	Room* roomConnected;
	string connectionName;
};

class Room {

public:

	Room(string roomName); 
	~Room(); 

	void CleanUp(); 

	string GetName() const; 
	void SetName(string newName); 

private: 

	string roomName;
	list<RoomLink*> roomLinks; 

};



#endif // !_ROOM_H_

