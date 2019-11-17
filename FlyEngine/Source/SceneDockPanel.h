#ifndef _SCENEDOCKPANEL_H_
#define _SCENEDOCKPANEL_H_

#include "DockPanel.h"
#include "Tool.h"

class Texture; 
class SceneDockPanel : public DockPanel {

public:

	SceneDockPanel(bool isVisible);
	~SceneDockPanel();

	bool Draw();

	void ShowViewportSettingsTab();

	// Object Creator Utility ---------------
	void DrawObjectCreator();
	void DrawSelectedToolSettings();
	void DrawToolImageSettings();
	void DrawCreatorBottomBar();
	void OnAddButtonClicked();
	void DrawCreatorAddAndDeleteButtons();
	void DrawCreatorToolsList();
	bool DrawToolSelectable(ToolSelectableInfo& toolInfo);

private: 
	std::list<Tool*> tmpToolsToAdd; 

	// Object Creator Vars -----------
	char searchToolBuffer[256] = ""; 
	char newObjectName[256] = ""; 
	
	bool showImageTool = false; 
	Texture* previewImageTexture = nullptr;

	ToolType toolAdjustmentsOwnerType = ToolType::AT_null; 
};

#endif 
