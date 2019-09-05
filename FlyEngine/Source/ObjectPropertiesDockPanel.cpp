#include "ObjectPropertiesDockPanel.h"

ObjectPropertiesDockPanel::ObjectPropertiesDockPanel(bool isVisible) : DockPanel("Object Properties", isVisible)
{
}

ObjectPropertiesDockPanel::~ObjectPropertiesDockPanel()
{
}

bool ObjectPropertiesDockPanel::Draw()
{
	return true; 
}
