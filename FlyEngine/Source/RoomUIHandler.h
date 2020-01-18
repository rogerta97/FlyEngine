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

	// Virtual ----------------------
	void CleanUp(); 
	void DrawUIElements(); 

	// Utility ----------------------
	void SetSelectedElement(UI_Element* newSelectedElement); 
	UI_Element* GetSelectedElement();

	// Factory ----------------------
	UI_Image* CreateUIImage(UID resourceUID); 

public: 
	std::list<UI_Element*> uiElements;

private: 
	Room* roomAttached; 
	UI_Element* selectedElement; 
};

#endif // !_ROOM_UI_HANDLER_H_

