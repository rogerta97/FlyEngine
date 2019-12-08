#include "ChangeSceeneAction.h"
#include "imgui.h"

#include "Application.h"
#include "ModuleImGui.h"

ChangeSceneAction::ChangeSceneAction(FlyObject* _parentObject)
{
	toolType = AT_CHANGE_SCENE;
	isVisual = false;

	SetActionName("Change Scene");
	SetToolDescription("This should be the description of the scene change");
}

ChangeSceneAction::~ChangeSceneAction()
{
}

void ChangeSceneAction::DrawActionOccurenceCheckboxes()
{
	ImGui::PushFont(App->moduleImGui->rudaBoldBig);
	ImGui::Text("Action Happens On:");
	ImGui::PopFont();

	ImGui::PushFont(App->moduleImGui->rudaRegularMid);
	ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0.12f, 0.14f, 0.17f, 1.00f));
	ImGui::BeginChild("##OccChild", ImVec2(ImGui::GetContentRegionAvailWidth(), 100));

	ImGui::SetCursorPos(ImVec2(5, 8));
	ImGui::Checkbox("Scene Enter", &occ_SceneEnter);
	ImGui::SetCursorPos(ImVec2(5, 38));
	ImGui::Checkbox("Scene Leave", &occ_SceneLeave);
	ImGui::SetCursorPos(ImVec2(5, 68));
	ImGui::Checkbox("Object Clicked", &occ_AreaClicked);

	ImGui::Spacing();
	ImGui::EndChild();

	ImGui::PopFont();
	ImGui::PopStyleColor();
}

void ChangeSceneAction::Init()
{
}

void ChangeSceneAction::Update()
{
}

void ChangeSceneAction::CleanUp()
{

}

void ChangeSceneAction::SetDestination(Room* dstRoom)
{
	destinationRoom = dstRoom; 
}

Room* ChangeSceneAction::GetDesination()
{
	return destinationRoom;
}
