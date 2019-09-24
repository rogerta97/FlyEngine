#ifndef _WORLD_PROPERTIES_DOCK_PANEL_H_
#define _WORLD_PROPERTIES_DOCK_PANEL_H_

#include "DockPanel.h"

class WorldPropertiesDockPanel : public DockPanel {

public:

	WorldPropertiesDockPanel(bool isVisible);
	~WorldPropertiesDockPanel();

	void PrintConnectionsSection(); 
	void PrintRoomsSection(); 

	void NewConnectionButtonHandler(); 
	void NewRoomButtonHandler(); 

	bool Draw();
};

#endif 

