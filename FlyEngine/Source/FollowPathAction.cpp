#include "FollowPathAction.h"
#include "imgui.h"
#include "Application.h"
#include "ModuleImGui.h"

using namespace ImGui; 

FollowPathAction::FollowPathAction(FlyObject* _parentObject)
{
	actionType = ACTION_FOLLOW_PATH;
	parentObject = _parentObject;
	isVisual = false;
	pathSteps = new std::list<PathStep*>(); 

	SetActionName("Follow Path");
	SetToolDescription("This should be the description of follow path action");
}

FollowPathAction::~FollowPathAction()
{
	
}

void FollowPathAction::DrawPath()
{

}

void FollowPathAction::DrawUISettings()
{
	if (ImGui::Button("Edit In Viewport"))
	{

	}

	ImGui::SameLine();

	int pathModeCombo = (int)pathMode; 
	if(ImGui::Combo("Play Mode", &pathModeCombo, "One Time\0Times Amount\0Loop Teleport\0Loop Cycle"))
	{
		pathMode = (PathMode)pathModeCombo;
	}

	if (ImGui::Button("Debug AddStep"))
	{
		PathStep* newStep = new PathStep();
		newStep->targetPosition = float2(0, 0);
		newStep->targetTime = 2000;
		pathSteps->push_back(newStep);
	}

	ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0.12f, 0.14f, 0.17f, 1.00f));
	ImGui::PushStyleColor(ImGuiCol_Separator, ImVec4(0.7f, 0.7f, 0.7f, 0.7f));
	ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(2.0f, 0.5f));

	ImGui::BeginChild("##PathsListHierarchy", ImVec2(ImGui::GetContentRegionAvailWidth(), 250));

	ImGui::PopStyleColor(2);
	ImGui::PopStyleVar();

	ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(2.0f, 2.0f));
	ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 5);
	ImGui::Spacing();

	if (pathSteps->size() <= 0)
	{
		INC_CURSOR_X_7;
		ImGui::PushFont(App->moduleImGui->rudaBlackGiant);
		ImGui::TextColored(ImVec4(1, 1, 1, 0.04f), "Empty");
		ImGui::PopFont();
	}
	else
	{
		int count = 0; 
		for (auto& currentStep : *pathSteps)
		{
			currentStep->DrawStepGUI(count);
			count++;
		}
	}

	ImGui::PopStyleVar();
	ImGui::EndChild();

}

void FollowPathAction::SaveAction(JSON_Object* jsonObject, string serializeObjectString, bool literalStr)
{
	string toolsSerializeSection;

	if (!literalStr)
		toolsSerializeSection = serializeObjectString + string("Actions.FollowPath.");
	else
		toolsSerializeSection = serializeObjectString;

	Action::SaveAction(jsonObject, toolsSerializeSection);
	Action::SaveOccurrence(jsonObject, toolsSerializeSection);
}

PathMode FollowPathAction::GetPathMode()
{
	return pathMode;
}

void FollowPathAction::SetPathMode(PathMode newPathMode)
{
	pathMode = newPathMode; 
}

void PathStep::DrawStepGUI(int stepPos, float selectableHeigth)
{
	PUSH_CHILD_BG_COLOR_DARK;
	string id = "PathStepSelectable_" + to_string(stepPos);

	ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 5);
	ImGui::BeginChild(id.c_str(), ImVec2(ImGui::GetContentRegionAvailWidth() - 5, selectableHeigth)); 

	ImGui::Columns(2);

	ImGui::SetColumnWidth(0, 70); 

	ImGui::SetCursorPos(ImVec2(23, 5));
	ImGui::PushFont(App->moduleImGui->rudaBlackGiant);
	ImGui::TextColored(ImVec4(1, 1, 1, 1.00f), to_string(stepPos).c_str());
	ImGui::PopFont();

	ImGui::NextColumn();

	float showPositionArr[2] = { targetPosition.x, targetPosition.y };
	ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 7);
	if (ImGui::InputFloat2("Target Position", showPositionArr, "%.2f"))
	{
		targetPosition = float2(showPositionArr[0], showPositionArr[1]);
	}

	if (ImGui::InputFloat("Target Time", &targetTime, 0.1f, 0.5f, 2))
	{

	}

	ImGui::EndChild(); 
	ImGui::PopStyleColor(); 
}
