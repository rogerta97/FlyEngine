#ifndef _SCENEDOCKPANEL_H_
#define _SCENEDOCKPANEL_H_

#include "DockPanel.h"
#include "Tool.h"
#include "Tool.h"

class ObjectCreator; 
class SceneDockPanel : public DockPanel {

public:

	SceneDockPanel(bool isVisible);
	~SceneDockPanel();

	bool Draw();

	void ShowViewportSettingsTab();

	// Object Creator -------------
	//void ShowObjectCreatorTab();
	//void DrawAddToolSection();
	//void DrawAddToolPopup();
	//void DrawSearchToolSelectable(const char* toolName , const char* toolDescription, ToolType toolType);

private: 

	ObjectCreator* objectCreator; 
	char searchToolBuffer[256] = ""; 

	bool addToolImage = false; 
	std::list<Tool*> tmpToolsToAdd; 
};

#endif 
