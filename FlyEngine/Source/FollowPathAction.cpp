#include "FollowPathAction.h"
#include "Texture.h"
#include "ResourceManager.h"
#include "imgui.h"
#include "Application.h"
#include "BoundingBox.h"
#include "ModuleImGui.h"
#include "FlyObject.h"
#include "FlyObjectInterpolator.h"
#include "ViewportManager.h"
#include "OpenGL.h"

using namespace ImGui; 

FollowPathAction::FollowPathAction(FlyObject* _parentObject)
{
	actionType = ACTION_FOLLOW_PATH;
	parentObject = _parentObject;
	isVisual = false;
	pathSteps = new std::list<PathStep*>(); 
	flyObjectInterpolation = new FlyObjectInterpolator(parentObject);
	movementState = MOVEMENT_IDLE; 

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

void FollowPathAction::Update(float dt)
{
	if (movementState == MOVEMENT_ONGOING)
	{
		stepTime += dt; 

		if (UpdateObjectPosition())
		{
			if (currentStepIndex >= pathSteps->size() - 1)
			{
				flog("The Path Has Finished :D"); 
				return; 
			}

			BeginNextStep(); 
		}
	}
}

void FollowPathAction::Draw()
{
	DrawPath(); 
}

void FollowPathAction::DoAction()
{
	BeginMovement(); 
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
			finishStepPos = currentStep->targetPosition * ViewportManager::getInstance()->GetAspectRatio(); 
			prevTargetPos = finishStepPos; 
		}
		else
		{
			startStepPos = prevTargetPos;
			finishStepPos = currentStep->targetPosition * ViewportManager::getInstance()->GetAspectRatio(); 
			prevTargetPos = finishStepPos; 
		}


		glLineWidth(lineWidth); 
		glColor4f(lineColor.x, lineColor.y, lineColor.z, lineColor.w);

		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();

		glBegin(GL_LINES);
		glVertex2f(startStepPos.x , startStepPos.y);
		glVertex2f(finishStepPos.x , finishStepPos.y);
		glEnd();

		glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
		glLineWidth(1.0f);

		count++;
	}

	for (auto currentStep : *pathSteps)
	{
		// Draw the box
		glColor4f(graphBoxColor.x, graphBoxColor.y, graphBoxColor.z, graphBoxColor.w);
		currentStep->graphBox->SetPosition(currentStep->targetPosition * ViewportManager::getInstance()->GetAspectRatio());
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

	ImGui::Separator();

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
		DoAction(); 
	}

	ImGui::SameLine();
	ImGui::PushFont(App->moduleImGui->rudaBlackBig);
	ImGui::TextColored(ImVec4(1, 1, 1, 1.0f), "Play Movement Preview");
	ImGui::PopFont();

	if (ImGui::Button("Add Step To Center"))
	{
		PathStep* newStep = new PathStep();
		newStep->targetPosition = float2(0, 0);
		newStep->SetMovementSpeed(5.0f);
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
	{
		if (pathSteps->empty())
			newStep->startPosition = parentObject->transform->GetPosition(true); 
		else
		{
			std::list<PathStep*>::iterator prevStep = pathSteps->begin();
			std::advance(prevStep, currentStepIndex - 1);
			newStep->startPosition = (*prevStep)->targetPosition; 
		}

		pathSteps->push_back(newStep); 
	}
}

void FollowPathAction::BeginMovement()
{
	if (movementState == MOVEMENT_ONGOING)
		return; 

	movementState = MOVEMENT_ONGOING;
	currentStepIndex = 0;
	stepTime = 0.0f;
	
	if (flyObjectInterpolation)
	{
		float2 startPosition = this->startPosition;
		float2 finishPosition = pathSteps->front()->targetPosition; 
		float targetTime = 5;

		flyObjectInterpolation->SetInterpolationSegment(startPosition, finishPosition); 
	}
}

// Will Return true if the object has reached the target point
bool FollowPathAction::UpdateObjectPosition()
{
	return flyObjectInterpolation->UpdateObjectPositionFromTime(stepTime);
}

void FollowPathAction::BeginNextStep()
{
	if (currentStepIndex >= pathSteps->size() - 1)
		return; 

	currentStepIndex++; 
	std::list<PathStep*>::iterator currentStep = pathSteps->begin();
	std::advance(currentStep, currentStepIndex);

	float2 startPosition = parentObject->transform->GetPosition(); 
	float2 finishPosition = (*currentStep)->targetPosition;
	float targetTime = 5;

	flyObjectInterpolation->SetInterpolationSegment(startPosition, finishPosition);
	stepTime = 0;
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
	startPosition = float2(0, 0); 
	targetPosition = float2(0, 0); 
	targetTime = 5.0f;
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
	json_object_dotset_number(jsonObject, string(serializeObjectString + "MovementSpeed").c_str(), speed);
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

	if (ImGui::DragFloat("Speed", &targetTime, 0.1f, 0.5f, 2))
	{

	}

	ImGui::EndChild(); 
	ImGui::PopStyleColor(); 
}

float PathStep::GetLenght()
{
	float2 diff = (targetPosition - startPosition);
	return diff.Length();
}

void PathStep::SetMovementSpeed(float _newSpeed)
{
	speed = _newSpeed; 
	float length = GetLenght(); 
	targetTime = speed / length; 
}
