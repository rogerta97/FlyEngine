#ifndef _GAME_PROPERTIES_DOCKPANEL_H_
#define _GAME_PROPERTIES_DOCKPANEL_H_

#include "DockPanel.h"
#include <vector>
#include <string>

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

	std::string GetProjectName(); 

private: 

	bool showNewRoomNaming = false;	

	Room* firstRoom; 
	std::string projectName;

};

#endif 

