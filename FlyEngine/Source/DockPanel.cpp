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

bool DockPanel::Draw() {

	// Security Checkings 
	assert(panelName != ""); // You should add a name to the docking window
	//assert(isMemberFrom != MEMBER_null); // Panels Should belong to a member 

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
