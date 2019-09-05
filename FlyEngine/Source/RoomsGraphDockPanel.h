#ifndef _ROOMSGRAPHDOCKPANEL_H_
#define _ROOMSGRAPHDOCKPANEL_H_

#include "DockPanel.h"

class RoomsGraphDockPanel : public DockPanel {

public:

	RoomsGraphDockPanel(bool isVisible);
	~RoomsGraphDockPanel();

	bool Draw();
};

#endif 
