#ifndef _ROOMSGRAPHDOCKPANEL_H_
#define _ROOMSGRAPHDOCKPANEL_H_

#include "DockPanel.h"
#include "NodeGraph.h"

class RoomsNodeGraph; 
class RoomsGraphDockPanel : public DockPanel {

public:

	RoomsGraphDockPanel(bool isVisible);
	~RoomsGraphDockPanel();

	bool Draw();
	bool CleanUp();

private:
	RoomsNodeGraph* roomsNodeGraph; 
	bool loaded = false; 
};

#endif 
