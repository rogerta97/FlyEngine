#ifndef _WORLD_PROPERTIES_DOCK_PANEL_H_
#define _WORLD_PROPERTIES_DOCK_PANEL_H_

#include "DockPanel.h"

class Room;
class WorldPropertiesDockPanel : public DockPanel {

public:

	WorldPropertiesDockPanel(bool isVisible);
	~WorldPropertiesDockPanel();

	void PrintRoomsSection(); 
	void ShowNewRoomUI(); 

	void NewConnectionButtonHandler(); 

	bool Draw();
	void PrintRoomInfo(Room* selectedRoom);

private:

	bool showNewRoomUI = false; 
	bool showNewOutConnectionUI = false; 
	bool showNewInConnectionUI = false; 
	string newRoomBuffer;
};

#endif 

