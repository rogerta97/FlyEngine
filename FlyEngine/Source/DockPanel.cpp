#include "DockPanel.h"
#include "imgui.h"
#include "mmgr.h"

DockPanel::DockPanel(std::string newPanelName, bool isVisible)
{
	panelName = newPanelName;
	visible = isVisible;
}

DockPanel::~DockPanel()
{
}

string DockPanel::GetName() const
{
	return panelName;
}

inline void DockPanel::SetName(string newName)
{
	panelName = newName;
}

DockPanelType DockPanel::GetPanelType() const
{
	return dockPanelType;
}

FlyEngineSection DockPanel::GetFlySection() const
{
	return flyEngineSection;
}

bool DockPanel::Draw() {

	// Security Checkings 
	assert(panelName != ""); // You should add a name to the docking window
	assert(flyEngineSection != FLY_SECTION_null); // Panels Should belong to a member 

	if (!visible)
		return false;

	return true; 
}

bool DockPanel::CleanUp()
{

	switch (dockPanelType)
	{
	case DOCK_CONSOLE:
		break;
	case DOCK_GAME_VIEWPORT:
		break;
	case DOCK_OBJECT_PROPERTIES:
		break;
	case DOCK_PROJECT_INFO:
		break;
	case DOCK_ROOM_OBJECTS:
		break;
	case DOCK_ROOMS_GRAPH:
		break;
	case DOCK_null:
		break;
	default:
		break;
	}

	return true;
}

// Will Return the new enabled state
bool DockPanel::ToggleVisibility()
{
	visible = !visible;
	return visible;
}

void DockPanel::SetVisible(bool newVisible)
{
	visible = newVisible; 
}
