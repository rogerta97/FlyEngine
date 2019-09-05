#ifndef _CONSOLEDOCKPANEL_H_
#define _CONSOLEDOCKPANEL_H_

#include "DockPanel.h"
class GameViewportDockPanel : public DockPanel
{
public: 

	GameViewportDockPanel(bool isVisible); 
	~GameViewportDockPanel(); 

	bool Draw();

};

#endif

