#include "DockPanel.h"

#include "imgui.h"

DockPanel::DockPanel(std::string newPanelName)
{
	panelName = newPanelName;
	visible = true;
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

void DockPanel::Draw() {}