#ifndef _OBJECTPROPERTIESDOCKPANEL_H_
#define _OBJECTPROPERTIESDOCKPANEL_H_

#include "DockPanel.h"

class FlyObject;
class ObjectCreator;
class ObjectPropertiesDockPanel : public DockPanel {

public:
	ObjectPropertiesDockPanel(bool isVisible);
	~ObjectPropertiesDockPanel();

	bool Draw();
	void DrawObjectTools(FlyObject* selectedObject);
	void DrawObjectPlacementCH(FlyObject* selectedObject);

	ObjectCreator* objectCreator = nullptr;
};

#endif 
