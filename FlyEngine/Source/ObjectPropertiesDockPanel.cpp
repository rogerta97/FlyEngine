#include "ObjectPropertiesDockPanel.h"
#include "imgui.h"
#include "mmgr.h"

ObjectPropertiesDockPanel::ObjectPropertiesDockPanel(bool isVisible) : DockPanel("Object Properties", isVisible)
{
	flyEngineSection = FLY_SECTION_ROOM_EDIT;
	dockPanelType = DOCK_OBJECT_PROPERTIES;
}

ObjectPropertiesDockPanel::~ObjectPropertiesDockPanel()
{
}

bool ObjectPropertiesDockPanel::Draw()
{

#pragma region secutiryChecks
	if (!DockPanel::Draw())
		return false;
#pragma endregion

	if (ImGui::Begin(panelName.c_str(), &visible)) {
		ImGui::Text("Im The Object Properties");
	}

	ImGui::End();
	return true;
}
