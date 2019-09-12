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

	Room(); 
	~Room(); 

private: 

	list<RoomLink*> roomLinks; 

};



#endif // !_ROOM_H_

