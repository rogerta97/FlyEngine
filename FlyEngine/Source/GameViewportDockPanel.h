#ifndef _GAMEVIEWPORTDOCKPANEL_H_
#define _GAMEVIEWPORTDOCKPANEL_H_

#include "DockPanel.h"
class GameViewportDockPanel : public DockPanel
{
public: 

	GameViewportDockPanel(bool isVisible); 
	~GameViewportDockPanel(); 

	bool Draw();

};

#endif

