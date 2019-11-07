#ifndef _DOCKPANEL_H_
#define _DOCKPANEL_H_

#include <string>
#include "Application.h"

using namespace std;

enum DockPanelType
{
	DOCK_CONSOLE,
	DOCK_GAME_VIEWPORT,
	DOCK_OBJECT_PROPERTIES,
	DOCK_WORLD_PROPERTIES,
	DOCK_PROJECT_INFO,
	DOCK_ROOM_OBJECTS,
	DOCK_SCENE, 
	DOCK_ROOMS_GRAPH,
	DOCK_null,
};

class DockPanel {

public:

	DockPanel(std::string newPanelName, bool isVisible);
	~DockPanel();

	virtual bool Draw();
	virtual void ReceiveEvent(FlyEngineEvent eventType);
	virtual bool CleanUp();

	// Utility
	bool ToggleVisibility();
	void SetVisible(bool newVisible); 
	bool& IsVisible(); 

	// Set & Get 
	string GetName() const;
	void inline SetName(string newName);

	DockPanelType GetPanelType() const;
	FlyEngineSection GetFlySection() const; 

protected:

	DockPanelType dockPanelType = DOCK_null; 
	FlyEngineSection flyEngineSection = FLY_SECTION_null;
	string panelName;
	bool visible;
};

#endif 

