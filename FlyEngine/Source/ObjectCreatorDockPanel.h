#ifndef _OBJECT_CREATOR_DOCK_PANEL_H_
#define _OBJECT_CREATOR_DOCK_PANEL_H_

#include "DockPanel.h"
#include "ModuleManager.h"

#include <string>

class FlyObject;
class ObjectCreatorDockPanel : public DockPanel {

public:
	ObjectCreatorDockPanel(bool isVisible);
	~ObjectCreatorDockPanel();

	void DrawObjectActionsList(); 
	void DrawSelectable(ToolSelectableInfo selectableInfo);

private:
	FlyObject* creatingObject = nullptr; 
};

#endif 

