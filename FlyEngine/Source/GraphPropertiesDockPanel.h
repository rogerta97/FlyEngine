#ifndef _GRAPH_PROPERTIES_DOCKPANEL_H_
#define _GRAPH_PROPERTIES_DOCKPANEL_H_

#include "DockPanel.h"
#include <vector>

class GraphPropertiesDockPanel : public DockPanel {

public:

	GraphPropertiesDockPanel(bool isVisible);
	~GraphPropertiesDockPanel();

	bool Draw();

	void NewRoomButtonHandler();
	void NewConnectionButtonHandler();

	void PrintRoomsSection();
	void PrintConnectionsSection();

private: 
	bool showNewRoomNaming = false;	
};

#endif 

