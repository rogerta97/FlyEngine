#include "ObjectPropertiesDockPanel.h"
#include "imgui.h"

ObjectPropertiesDockPanel::ObjectPropertiesDockPanel(bool isVisible) : DockPanel("Object Properties", isVisible)
{
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
