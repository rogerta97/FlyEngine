#ifndef _ROOMDOCKPANEL_H_
#define _ROOMDOCKPANEL_H_

#include "DockPanel.h"
#include "Tool.h"

class Texture; 
class RoomDockPanel : public DockPanel {

public:

	RoomDockPanel(bool isVisible);
	~RoomDockPanel();

	bool Draw();

	void ShowViewportSettingsTab();

private: 
	std::list<Tool*> tmpToolsToAdd; 

};

#endif 
