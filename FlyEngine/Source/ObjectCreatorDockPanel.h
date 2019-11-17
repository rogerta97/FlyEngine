#ifndef _OBJECT_CREATOR_DOCK_PANEL_H_
#define _OBJECT_CREATOR_DOCK_PANEL_H_

#include "DockPanel.h"
#include "ModuleManager.h"

#include <string>

class FlyObject;
class Tool; 
class ObjectCreatorDockPanel : public DockPanel {

public:
	ObjectCreatorDockPanel(bool isVisible);
	~ObjectCreatorDockPanel();

	bool Draw(); 

	void ResetObjectData();
	void Close(); 

	void DrawObjectCreator(); 
	void DrawObjectToolsList(); 
	void DrawSelectable(ToolSelectableInfo selectableInfo, bool& isSelected, Tool* currentTool);
	void DrawAddAndDeleteToolButtons(); 
	void DrawToolImageSettings(); 
	void DrawSelectedToolSettings(); 
	void OnAddToolButtonClicked(); 
	void DrawCreateButton(); 

private:

	char searchNewToolBuffer[256]; 
	char newObjectName[256]; 

	FlyObject* creatingObject = nullptr; 
	Tool* selectedTool = nullptr; 
};

#endif 

