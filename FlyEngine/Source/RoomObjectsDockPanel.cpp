#include "RoomObjectsDockPanel.h"

RoomObjectsDockPanel::RoomObjectsDockPanel(bool isVisible) : DockPanel("Room Objects", isVisible)
{
}

RoomObjectsDockPanel::~RoomObjectsDockPanel()
{
}

bool RoomObjectsDockPanel::Draw()
{
	return true;
}
