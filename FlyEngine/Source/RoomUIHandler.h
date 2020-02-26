#ifndef _ROOM_UI_HANDLER_H_
#define _ROOM_UI_HANDLER_H_

#include "Globals.h"
#include "SaveAndLoad.h"
#include "UI_Element.h"
#include <list>
#include <string>

class Room; 
class UI_Image; 
class UI_Button; 
class UI_Text; 
class Action; 
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
	void AddEmptyElement(UIElementType elementType);
	void SetSelectedElement(UI_Element* newSelectedElement); 
	void SetSelectedElement(UID newSelectedElementUID); 
	UI_Element* GetUIElement(UID elementUID);
	UI_Element* GetSelectedElement();

	// Draw --------------------------
	UID DrawUIElementsHierarchy(); 
	void DrawSelectedOnClickActionSettings(); 
	UIElementType DrawUIElementSelectorPopup();

	// Factory ----------------------
	UI_Image* CreateUIImage(UID resourceUID); 
	UI_Image* CreateUIImage(); 
	UI_Button* CreateUIButton(); 
	UI_Text* CreateUIText();

	void DeleteElement(UID elementUID); 

public: 
	std::list<UI_Element*> uiElements;

	// ImGui variables Button -------
	Action* selectedButtonUIAction;

private: 
	Room* roomAttached; 
	UI_Element* selectedElement; 
};

#endif // !_ROOM_UI_HANDLER_H_

