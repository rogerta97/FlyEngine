#ifndef _ROOM_UI_HANDLER_H_
#define _ROOM_UI_HANDLER_H_

class Room; 
class RoomUIHandler
{
public: 
	RoomUIHandler(Room* roomAttached); 
	~RoomUIHandler();

private: 
	Room* roomAttached; 
};

#endif // !_ROOM_UI_HANDLER_H_

