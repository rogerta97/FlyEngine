#ifndef _GLOBAL_BLACKBOARD_DOCK_PANEL_H_
#define _GLOBAL_BLACKBOARD_DOCK_PANEL_H_

#include "DockPanel.h"

class GlobalBlackboardDockPanel : public DockPanel 
{
public:

	GlobalBlackboardDockPanel(bool isVisible);
	~GlobalBlackboardDockPanel();

	bool Draw();

private:

};


#endif

