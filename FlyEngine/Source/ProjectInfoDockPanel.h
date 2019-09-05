#ifndef _PROJECTINFODOCKPANEL_H_
#define _PROJECTINFODOCKPANEL_H_

#include "DockPanel.h"

class ProjectInfoDockPanel : public DockPanel {

public:

	ProjectInfoDockPanel(bool isVisible);
	~ProjectInfoDockPanel();

	bool Draw();

private:

	int messagesAmmount;
};

#endif 

