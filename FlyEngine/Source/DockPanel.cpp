#include "DockPanel.h"
#include "imgui.h"

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
