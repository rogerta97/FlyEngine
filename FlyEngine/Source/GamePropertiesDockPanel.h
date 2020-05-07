#ifndef _GAME_PROPERTIES_DOCKPANEL_H_
#define _GAME_PROPERTIES_DOCKPANEL_H_

#include "DockPanel.h"
#include <vector>

class Room;
class GamePropertiesDockPanel : public DockPanel {

public:

	GamePropertiesDockPanel(bool isVisible);
	~GamePropertiesDockPanel();

	bool Draw();

	void NewRoomButtonHandler();
	void NewConnectionButtonHandler();

	void PrintRoomsSection();
	void PrintConnectionsSection();

private: 

	bool showNewRoomNaming = false;	

	Room* firstRoom; 

};

#endif 

