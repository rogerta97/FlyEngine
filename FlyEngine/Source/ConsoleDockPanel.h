#ifndef _CONSOLEDOCKPANEL_H_
#define _CONSOLEDOCKPANEL_H_

#include "DockPanel.h"

#include <map>

using namespace std; 

enum consoleMessageType {
	CM_DEBUG,
	CM_WARNING,
	CM_ERROR
};

class ConsoleDockPanel : public DockPanel {

public: 

	ConsoleDockPanel(bool isVisible);
	~ConsoleDockPanel(); 

	bool Draw(); 

private: 

	multimap<consoleMessageType, const char*> messagesList; 

};

#endif 
