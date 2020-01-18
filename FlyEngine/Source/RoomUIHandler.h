#ifndef _ROOM_UI_HANDLER_H_
#define _ROOM_UI_HANDLER_H_

#include "Globals.h"
#include <list>

class Room; 
class UI_Element; 
class UI_Image; 
class RoomUIHandler
{
public: 
	RoomUIHandler(Room* roomAttached); 
	~RoomUIHandler();

	void DrawUIElements(); 

	UI_Image* CreateUIImage(UID resourceUID); 

private: 
	Room* roomAttached; 
	std::list<UI_Element*> uiElements;
};

#endif // !_ROOM_UI_HANDLER_H_

