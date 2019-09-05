#ifndef _PROJECTINFODOCKPANEL_H_
#define _PROJECTINFODOCKPANEL_H_

#include "DockPanel.h"

class ProjectInfoDockPanel : public DockPanel {

public:

	ProjectInfoDockPanel();
	~ProjectInfoDockPanel();

	void Draw();

private:

	int messagesAmmount;
};

#endif 

