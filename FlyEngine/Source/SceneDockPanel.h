#ifndef _SCENEDOCKPANEL_H_
#define _SCENEDOCKPANEL_H_

#include "DockPanel.h"

class SceneDockPanel : public DockPanel {

public:

	SceneDockPanel(bool isVisible);
	~SceneDockPanel();

	bool Draw();
	void ShowObjectCreatorTab();
	void ShowViewportSettingsTab();
};

#endif 
