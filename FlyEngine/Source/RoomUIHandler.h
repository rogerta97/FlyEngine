#ifndef _ROOM_UI_HANDLER_H_
#define _ROOM_UI_HANDLER_H_

#include "Globals.h"
#include "SaveAndLoad.h"
#include <list>
#include <string>

class Room; 
class UI_Element; 
class UI_Image; 
class UI_Button; 
class RoomUIHandler
{
public: 
	RoomUIHandler(Room* roomAttached); 
	~RoomUIHandler();

	// Save & Load 
	void SaveRoomUI(JSON_Object* jsonObject, std::string serializeStr); 
	void LoadRoomUI(JSON_Object* jsonObject, std::string serializeStr); 

	// Virtual ----------------------
	void Update(); 
	void CleanUp(); 
	void DrawUIElements(); 

	// Utility ----------------------
	void SetSelectedElement(UI_Element* newSelectedElement); 
	UI_Element* GetSelectedElement();

	// Factory ----------------------
	UI_Image* CreateUIImage(UID resourceUID); 
	UI_Image* CreateUIImage(); 
	UI_Button* CreateUIButton(); 

	void DeleteElement(UID elementUID); 

public: 
	std::list<UI_Element*> uiElements;

private: 
	Room* roomAttached; 
	UI_Element* selectedElement; 
};

#endif // !_ROOM_UI_HANDLER_H_

