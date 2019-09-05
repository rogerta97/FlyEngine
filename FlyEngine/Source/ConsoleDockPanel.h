#ifndef _CONSOLEDOCKPANEL_H_
#define _CONSOLEDOCKPANEL_H_

#include "DockPanel.h"

class ConsoleDockPanel : public DockPanel {

public: 

	ConsoleDockPanel(bool isVisible);
	~ConsoleDockPanel(); 

	bool Draw(); 

private: 

	int messagesAmmount; 
};

#endif 
