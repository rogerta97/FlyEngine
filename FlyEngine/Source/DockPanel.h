#ifndef _DOCKPANEL_H_
#define _DOCKPANEL_H_

#include <string>

using namespace std;

enum FlyEngineSection 
{
	FLY_SECTION_ROOM_GRAPH,
	FLY_SECTION_ROOM_EDIT,
	FLY_SECTION_BOTH,
	FLY_SECTION_null
};

enum DockPanelType
{
	DOCK_CONSOLE,
	DOCK_GAME_VIEWPORT,
	DOCK_OBJECT_PROPERTIES,
	DOCK_PROJECT_INFO,
	DOCK_ROOM_OBJECTS,
	DOCK_ROOMS_GRAPH,
	DOCK_null,
};

class DockPanel {

public:

	DockPanel(std::string newPanelName, bool isVisible);
	~DockPanel();

	virtual bool Draw();

	// Utility
	bool ToggleVisibility();
	void SetVisible(bool newVisible); 

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

