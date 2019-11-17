#include "ObjectCreatorDockPanel.h"

#include "Application.h"
#include "ModuleImGui.h"
#include "Tool.h"

#include "FlyObject.h"
#include "imgui.h"

ObjectCreatorDockPanel::ObjectCreatorDockPanel(bool isVisible) : DockPanel("Console", isVisible)
{
	flyEngineSection = FLY_SECTION_ROOM_EDIT;
	dockPanelType = DOCK_OBJECT_CREATOR;
}

ObjectCreatorDockPanel::~ObjectCreatorDockPanel()
{

}

void ObjectCreatorDockPanel::DrawObjectActionsList()
{
	ImGui::PushFont(App->moduleImGui->rudaBoldBig);
	ImGui::Text("Add Object Actions: ");
	ImGui::PopFont();

	ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0.12f, 0.14f, 0.17f, 1.00f));
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(5, 5));

	ImGui::BeginChild("##AttributesChild", ImVec2(ImGui::GetContentRegionAvailWidth(), 200));

	for (auto& currentTool : creatingObject->GetToolsList())
	{
		ToolSelectableInfo selectableInfo = currentTool->GetToolSelectableInfo();
		DrawSelectable(selectableInfo, currentTool->isSelected);
	}

	ImGui::EndChild();

	ImGui::PopStyleVar();
	ImGui::PopStyleColor();
}

void ObjectCreatorDockPanel::DrawSelectable(ToolSelectableInfo selectableInfo)
{
	ImGui::PushFont(App->moduleImGui->rudaBoldMid);
	if (ImGui::Selectable(selectableInfo.toolName.c_str(), &currentTool->isSelected, ImGuiSelectableFlags_None, ImVec2(ImGui::GetContentRegionAvailWidth(), 37))) {
		selectedObject->selectedTool = currentTool;
	}
	ImGui::PopFont();

	// Description -----
	ImGui::SetCursorPosY(ImGui::GetCursorPos().y - 20);
	ImGui::SetCursorPosX(ImGui::GetCursorPos().x + 2);

	ImGui::PushFont(App->moduleImGui->rudaRegularSmall);
	ImGui::TextWrapped(selectableInfo.toolDescription.c_str());
	ImGui::PopFont();
}
