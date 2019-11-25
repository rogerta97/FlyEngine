#ifndef _OBJECT_PROPERTIES_DOCK_PANEL_H_
#define _OBJECT_PROPERTIES_DOCK_PANEL_H_

#include "DockPanel.h"
#include "ModuleManager.h"

class FlyObject;
class ObjectPropertiesDockPanel : public DockPanel {

public:
	ObjectPropertiesDockPanel(bool isVisible);
	~ObjectPropertiesDockPanel();

	bool Draw();

	void SetSelectedObject(FlyObject* newSelectedObject);
	FlyObject* GetSelectedObject() const;

	void DrawObjectTools();
	void DrawToolAdjustments();
	void DrawToolList();
	void DrawAddAndDeleteButtons();
	void DrawToolSelectable(ToolSelectableInfo& selectableInfo, Tool*& currentTool);
	void DrawObjectPlacementCH();
	void DrawToolImageSettings();

private: 
	char searchNewToolBuffer[256]; 
	FlyObject* selectedObject = nullptr; 
	bool showToolDictionary; 
};

#endif 
