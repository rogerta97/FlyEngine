#ifndef _ROOMSGRAPHDOCKPANEL_H_
#define _ROOMSGRAPHDOCKPANEL_H_

#include "DockPanel.h"
#include "NodeGraph.h"

class RoomsGraphDockPanel : public DockPanel {

public:

	RoomsGraphDockPanel(bool isVisible);
	~RoomsGraphDockPanel();

	bool Draw();

private:
	NodeGraph nodeGraph; 
};

#endif 
