#ifndef _ROOMOBJECTSDOCKPANEL_H_
#define _ROOMOBJECTSDOCKPANEL_H_

#include "DockPanel.h"

class RoomObjectsDockPanel : public DockPanel {

public:

	RoomObjectsDockPanel(bool isVisible);
	~RoomObjectsDockPanel();

	bool Draw();

};

#endif 
