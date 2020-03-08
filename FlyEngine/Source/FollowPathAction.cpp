#include "FollowPathAction.h"
#include "Texture.h"
#include "ResourceManager.h"
#include "imgui.h"
#include "Application.h"
#include "BoundingBox.h"
#include "ModuleImGui.h"
#include "FlyObject.h"
#include "OpenGL.h"

using namespace ImGui; 

FollowPathAction::FollowPathAction(FlyObject* _parentObject)
{
	actionType = ACTION_FOLLOW_PATH;
	parentObject = _parentObject;
	isVisual = false;
	pathSteps = new std::list<PathStep*>(); 

	startPosition = parentObject->transform->GetPosition();
	graphBoxColor = float4(1.0f, 1.0f, 1.0f, 1.0f);
	lineColor = float4(1.0f, 1.0f, 1.0f, 1.0f);
	lineWidth = 5.0f; 

	SetActionName("Follow Path");
	SetToolDescription("This should be the description of follow path action");
}

FollowPathAction::~FollowPathAction()
{
	
}

void FollowPathAction::Draw()
{
	DrawPath(); 
}

void FollowPathAction::DrawPath()
{
	int count = 0; 
	float2 prevTargetPos = float2(0, 0); 
	for (auto currentStep : *pathSteps)
	{
		float2 startStepPos = float2(0, 0);
		float2 finishStepPos = float2(0, 0);

		// Draw the line from the desired position to the next one 
		if (count == 0)
		{
			startStepPos = startPosition; 
			finishStepPos = currentStep->targetPosition; 
			prevTargetPos = finishStepPos; 
		}
		else
		{
			startStepPos = prevTargetPos;
			finishStepPos = currentStep->targetPosition; 
			prevTargetPos = finishStepPos; 
		}

		glLineWidth(lineWidth); 
		glColor4f(lineColor.x, lineColor.y, lineColor.z, lineColor.w);

		glBegin(GL_LINES);
		glVertex2f(startStepPos.x, startStepPos.y);
		glVertex2f(finishStepPos.x, finishStepPos.y);
		glEnd();

		glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
		glLineWidth(1.0f);

		count++;
	}

	for (auto currentStep : *pathSteps)
	{
		// Draw the box
		glColor4f(graphBoxColor.x, graphBoxColor.y, graphBoxColor.z, graphBoxColor.w);
		currentStep->graphBox->SetPosition(currentStep->targetPosition);
		currentStep->graphBox->Draw(true, float4(0.0f, 1.0f, 0.0f, 1.0f)); 
		glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
	}
}

void FollowPathAction::DrawUISettings()
{
	ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0.12f, 0.14f, 0.17f, 1.00f));
	ImGui::BeginChild("##PathSettings", ImVec2(ImGui::GetContentRegionAvailWidth(), 120));

	INC_CURSOR_X_7;
	ImGui::PushFont(App->moduleImGui->rudaBlackBig);
	ImGui::TextColored(ImVec4(1, 1, 1, 1.0f), "Visuals:");
	ImGui::PopFont();

	DrawVisualSettings();

	ImGui::PopStyleColor();
	ImGui::EndChild();

	ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0.12f, 0.14f, 0.17f, 1.00f));

	ImGui::PushFont(App->moduleImGui->rudaBlackHuge);
	ImGui::TextColored(ImVec4(1, 1, 1, 1.0f), "Steps List:");
	ImGui::PopFont();

	DrawBehaviorSettings();

	ImGui::BeginChild("##PathsListHierarchy", ImVec2(ImGui::GetContentRegionAvailWidth(), 250));

	ImGui::PopStyleColor();

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

void FollowPathAction::DrawBehaviorSettings()
{
	Texture* playTexture = ResourceManager::getInstance()->GetTexture("PlayIcon");
	if (ImGui::ImageButton((ImTextureID)playTexture->GetTextureID(), ImVec2(30, 30)))
	{

	}

	ImGui::SameLine();
	ImGui::PushFont(App->moduleImGui->rudaBlackBig);
	ImGui::TextColored(ImVec4(1, 1, 1, 1.0f), "Play Movement Preview");
	ImGui::PopFont();


	if (ImGui::Button("Add Step To Center"))
	{
		PathStep* newStep = new PathStep();
		newStep->targetPosition = float2(0, 0);
		newStep->targetTime = 2000;
		pathSteps->push_back(newStep);
	}

	ImGui::SameLine();
	if (ImGui::Button("Edit In Viewport"))
	{

	}

	int pathModeCombo = (int)pathPlayMode;
	if (ImGui::Combo("Play Mode", &pathModeCombo, "One Time\0Times Amount\0Loop Teleport\0Loop Cycle"))
	{
		pathPlayMode = (PathPlayMode)pathModeCombo;
	}
}

void FollowPathAction::DrawVisualSettings()
{
	INC_CURSOR_X_7;
	float lineColorArr[3] = { lineColor.x, lineColor.y, lineColor.z };
	if (ImGui::ColorEdit3("Line Color", lineColorArr, 0))
	{
		lineColor = float4(lineColorArr[0], lineColorArr[1], lineColorArr[2], 1.0f);
	}

	INC_CURSOR_X_7;
	if (ImGui::InputFloat("Line Width", &lineWidth, 0.1f, 0.3f, 1))
	{

	}

	INC_CURSOR_X_7;
	float boxColorArr[3] = { graphBoxColor.x, graphBoxColor.y, graphBoxColor.z };
	if (ImGui::ColorEdit3("Box Color", boxColorArr, 0))
	{
		graphBoxColor = float4(boxColorArr[0], boxColorArr[1], boxColorArr[2], 1.0f);
	}
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

	// Save Reproduce type 
	json_object_dotset_number(jsonObject, string(toolsSerializeSection + "PlayMode").c_str(), (int)pathPlayMode);

	// Save Steps 
	string stepSerializeGroupStr = toolsSerializeSection + "PathSteps."; 
	json_object_dotset_number(jsonObject, string(stepSerializeGroupStr + "StepsAmount").c_str(), pathSteps->size());

	int count = 0; 
	for(auto& currentStep : *pathSteps)
	{
		string serializeStepStr = stepSerializeGroupStr + "Step_" + to_string(count) + ".";		
		currentStep->Save(jsonObject, serializeStepStr);
		count++; 
	}
}

void FollowPathAction::AddStep(PathStep* newStep)
{
	if (newStep != nullptr)
		pathSteps->push_back(newStep); 
}


PathPlayMode FollowPathAction::GetPathMode()
{
	return pathPlayMode;
}

void FollowPathAction::SetPathMode(PathPlayMode newPathMode)
{
	pathPlayMode = newPathMode;
}

PathStep::PathStep()
{
	targetPosition = float2(0, 0); 
	targetTime = 1000.0f;
	graphBox = new BoundingBox();
	graphBox->SetSize(25.0f, 25.0f);
}

PathStep::~PathStep()
{
}

void PathStep::Save(JSON_Object* jsonObject, string serializeObjectString)
{
	json_object_dotset_number(jsonObject, string(serializeObjectString + "TargetPosition.x").c_str(), targetPosition.x);
	json_object_dotset_number(jsonObject, string(serializeObjectString + "TargetPosition.y").c_str(), targetPosition.y);
	json_object_dotset_number(jsonObject, string(serializeObjectString + "TargetTime").c_str(), targetTime);
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
	if (ImGui::DragFloat2("Move To", showPositionArr, 1.0f, -500.0f, 500.0f, "%.2f"))
	{
		targetPosition = float2(showPositionArr[0], showPositionArr[1]);
	}

	if (ImGui::DragFloat2("Time To Move", &targetTime, 0.1f, 0.5f, 2))
	{

	}

	ImGui::EndChild(); 
	ImGui::PopStyleColor(); 
}
