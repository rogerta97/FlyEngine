#ifndef _OBJECTPROPERTIESDOCKPANEL_H_
#define _OBJECTPROPERTIESDOCKPANEL_H_

#include "DockPanel.h"
#include "ModuleManager.h"

class FlyObject;
class ObjectPropertiesDockPanel : public DockPanel {

public:
	ObjectPropertiesDockPanel(bool isVisible);
	~ObjectPropertiesDockPanel();

	bool Draw();

	void DrawObjectTools(FlyObject* selectedObject);
	void DrawToolAdjustments(FlyObject* selectedObject);
	void DrawToolList(FlyObject* selectedObject);
	void DrawAddAndDeleteButtons(FlyObject* selectedObject);
	void DrawToolSelectable(ToolSelectableInfo& selectableInfo, Tool*& currentTool, FlyObject* selectedObject);
	void DrawObjectPlacementCH(FlyObject* selectedObject);
	void DrawToolImageSettings(FlyObject* selectedObject);
};

#endif 
