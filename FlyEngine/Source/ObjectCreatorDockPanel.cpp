#include "ObjectCreatorDockPanel.h"
#include "imgui.h"

ObjectCreatorDockPanel::ObjectCreatorDockPanel(bool isVisible) : DockPanel("Console", isVisible)
{
	flyEngineSection = FLY_SECTION_ROOM_EDIT;
	dockPanelType = DOCK_OBJECT_CREATOR;
}

ObjectCreatorDockPanel::~ObjectCreatorDockPanel()
{

}
