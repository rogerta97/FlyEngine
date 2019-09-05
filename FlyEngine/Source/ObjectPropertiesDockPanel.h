#ifndef _OBJECTPROPERTIESDOCKPANEL_H_
#define _OBJECTPROPERTIESDOCKPANEL_H_

#include "DockPanel.h"

class ObjectPropertiesDockPanel : public DockPanel {

public:

	ObjectPropertiesDockPanel(bool isVisible);
	~ObjectPropertiesDockPanel();

	bool Draw();
};

#endif 
