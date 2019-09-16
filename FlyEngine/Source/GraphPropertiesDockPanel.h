#ifndef _GRAPH_PROPERTIES_DOCKPANEL_H_
#define _GRAPH_PROPERTIES_DOCKPANEL_H_

#include "DockPanel.h"

class GraphPropertiesDockPanel : public DockPanel {

public:

	GraphPropertiesDockPanel(bool isVisible);
	~GraphPropertiesDockPanel();

	bool Draw();

};

#endif 

