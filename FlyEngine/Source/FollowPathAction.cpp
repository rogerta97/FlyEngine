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

#include "mmgr.h"

using namespace ImGui; 

FollowPathAction::FollowPathAction(FlyObject* _parentObject)
{
	actionType = ACTION_FOLLOW_PATH;
	parentObject = _parentObject;
	isVisual = false;
	pathSteps = new std::list<PathStep*>(); 
	flyObjectInterpolation = new FlyObjectInterpolator(parentObject);
	movementState = MOVEMENT_IDLE; 
	loopsCompleted = 0; 
	targetLoopsAmount = 2;
	constantSpeed = 0; 
	isSpeedConstant = false; 

	isValidForCreator = false; 

	startBox = new BoundingBox();
	startBox->SetSize(25, 25); 

	startPosition = float2(0,0);
	lineWidth = 5.0f; 
	alphaFactor = 0.2f;

	graphBoxColor = float4(0.0f, 1.0f, 0.0f, 1.0f);
	lineColor = float4(1.0f, 1.0f, 1.0f, 1.0f);
	startBoxColor = float4(1.0f, 0.0f, 0.0f, 1.0f);

	SetActionName("Follow Path");
	SetToolDescription("This should be the description of follow path action");
}

FollowPathAction::FollowPathAction(FollowPathAction* otherAction)
{
	this->currentStepIndex = otherAction->currentStepIndex;
	this->stepTime = otherAction->stepTime;
	this->constantSpeed = otherAction->constantSpeed;
	this->isSpeedConstant = otherAction->isSpeedConstant;

	this->loopsCompleted = otherAction->loopsCompleted;
	this->targetLoopsAmount = otherAction->targetLoopsAmount;

	this->startPosition = otherAction->startPosition;
	this->lineWidth = otherAction->lineWidth;
	this->alphaFactor = otherAction->alphaFactor;

	this->graphBoxColor = otherAction->graphBoxColor;
	this->startBoxColor = otherAction->startBoxColor;
	this->lineColor = otherAction->lineColor;
}

FollowPathAction::~FollowPathAction()
{
	
}

void FollowPathAction::Update(float dt)
{
	if (movementState == MOVEMENT_ONGOING || movementState == MOVEMENT_CYCLE)
	{
		stepTime += dt; 

		if (UpdateObjectPosition())
		{
			if (movementState == MOVEMENT_CYCLE)
			{
				ResetPathMovement();
				BeginMovement();
				return;
			}

			if (currentStepIndex >= pathSteps->size() - 1)
			{
				switch (pathPlayMode)
				{
				case PATH_PLAY_ONCE: 
				{
					bool goToStart = true; 

					if (App->isEngineInPlayMode)
						goToStart = false; 

					Stop(goToStart);
					SetActionCompleted(true); 
				}
					break; 

				case PATH_LOOP_TIMES:
				{
					if (loopsCompleted < targetLoopsAmount - 1)
					{
						loopsCompleted++;
						Stop(true);
						BeginMovement();
					}
					else
					{
						Stop(true); 
						SetActionCompleted(true);
					}
				}
					break;

				case PATH_LOOP_TELEPORT:
				{
					Stop(true);
					BeginMovement();
				}
					break;

				case PATH_LOOP_CYCLE:
				{
					float2 startPosition = pathSteps->back()->targetPosition;
					float2 finishPosition = this->startPosition;
					float targetTime = 5;

					flyObjectInterpolation->SetInterpolationSegment(startPosition, finishPosition);
					stepTime = 0;
					movementState = MOVEMENT_CYCLE;
				}
					break;
				}

				return; 
			}

			if(currentStepIndex == -1)
				BeginMovement();
			else
				BeginNextStep(); 
		}
	}
}

void FollowPathAction::Stop(bool goToStart)
{
	if(goToStart)
		ResetPathMovement();

	movementState = MOVEMENT_IDLE;
}

void FollowPathAction::SetAlphaFactor(float _alphaFactor)
{
	alphaFactor = _alphaFactor;
}

void FollowPathAction::Draw()
{
	if(parentObject->isSelected)
		DrawPath(); 
}

void FollowPathAction::DoAction()
{
	loopsCompleted = 0;
	BeginMovement(); 
}

void FollowPathAction::CleanUp()
{
	for (auto& currentStep : *pathSteps)
	{
		currentStep->CleanUp(); 
		delete currentStep; 
	}

	pathSteps->clear();
	delete pathSteps; 

	flyObjectInterpolation->CleanUp();
	delete flyObjectInterpolation; 

	startBox->CleanUp();
	delete startBox; 
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
			startStepPos = startPosition * ViewportManager::getInstance()->GetAspectRatio();
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
	
		glColor4f(lineColor.x, lineColor.y, lineColor.z, alphaFactor);

		if(isSelected)
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
		currentStep->graphBox->SetPosition(currentStep->targetPosition * ViewportManager::getInstance()->GetAspectRatio());

		if (isSelected)
			currentStep->graphBox->Draw(true, float4(graphBoxColor.x, graphBoxColor.y, graphBoxColor.z, graphBoxColor.w));
		else
			currentStep->graphBox->Draw(true, float4(graphBoxColor.x, graphBoxColor.y, graphBoxColor.z, alphaFactor));

		//glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
	}

	// Draw Starting Box 
	startBox->SetPosition(startPosition * ViewportManager::getInstance()->GetAspectRatio());

	if (isSelected)
		startBox->Draw(true, float4(startBoxColor.x, startBoxColor.y, startBoxColor.z, startBoxColor.w));
	else
		startBox->Draw(true, float4(startBoxColor.x, startBoxColor.y, startBoxColor.z, alphaFactor));

	//glColor4f(1.0f, 1.0f, 1.0f, 1.0f);

}

void FollowPathAction::DrawActionOccurenceCheckboxes()
{
	ImGui::PushFont(App->moduleImGui->rudaBoldBig);
	ImGui::Text("Action Happens On:");
	ImGui::PopFont();

	ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0.12f, 0.14f, 0.17f, 1.00f));
	
	if (GetActionClass() == ACTION_CLASS_SEQUENTIAL)
	{
		ImGui::BeginChild("##OccChild", ImVec2(ImGui::GetContentRegionAvailWidth(), 35));
		ImGui::SetCursorPos(ImVec2(5, 5));
		ImGui::Checkbox("Object Condition", &occ_blackboardValue);
	}
	else if (GetActionClass() == ACTION_CLASS_DIRECT)
	{
		ImGui::BeginChild("##OccChild", ImVec2(ImGui::GetContentRegionAvailWidth(), 95));
		ImGui::SetCursorPos(ImVec2(5, 5));
		ImGui::Checkbox("Scene Enter", &occ_SceneEnter);
		ImGui::SetCursorPos(ImVec2(5, 35));
		ImGui::Checkbox("Object Clicked", &occ_ObjectClicked);
		ImGui::SetCursorPos(ImVec2(5, 65));
		ImGui::Checkbox("Object Condition", &occ_blackboardValue);
	}

	ImGui::SameLine();
	static std::string showValueConditionButtonText = "Show Conditions";
	if (ImGui::Button(showValueConditionButtonText.c_str()))
	{
		if (showVariableConditions)
		{
			showVariableConditions = false;
			showValueConditionButtonText = "Show Conditions";
		}
		else
		{
			showVariableConditions = true;
			showValueConditionButtonText = "Hide Conditions";
		}
	}

	ImGui::EndChild();
	ImGui::Spacing();

	if (showVariableConditions)
		DrawActionConditionsList();

	ImGui::PopStyleColor();

}


void FollowPathAction::DrawUISettings()
{
	if (ImGui::CollapsingHeader("Emit Sound Settings", ImGuiTreeNodeFlags_DefaultOpen))
	{
		DrawActionOccurenceCheckboxes(); 

		ImGui::Separator(); 

		ImGui::PushFont(App->moduleImGui->rudaBlackBig);
		ImGui::TextColored(ImVec4(1, 1, 1, 1.0f), "Visuals:");
		ImGui::PopFont();

		ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0.12f, 0.14f, 0.17f, 1.00f));
		ImGui::BeginChild("##PathSettings", ImVec2(ImGui::GetContentRegionAvailWidth(), 100));

		DrawVisualSettings();

		ImGui::PopStyleColor();
		ImGui::EndChild();

		ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0.12f, 0.14f, 0.17f, 1.00f));

		ImGui::Separator();

		DrawBehaviorSettings();

		ImGui::PushFont(App->moduleImGui->rudaBlackBig);
		ImGui::TextColored(ImVec4(1, 1, 1, 1.0f), "Steps List:");
		ImGui::PopFont();

		ImGui::BeginChild("##PathsListHierarchy", ImVec2(ImGui::GetContentRegionAvailWidth(), 250));

		PUSH_CHILD_BG_COLOR_DARK;
		INC_CURSOR_4;
		ImGui::BeginChild("StartPosPathDark", ImVec2(ImGui::GetContentRegionAvailWidth() - 5, 38));

		float startPos[2] = { startPosition.x, startPosition.y }; 
		INC_CURSOR_7;
		if (ImGui::DragFloat2("Start Position", startPos, 0.5f, -500.0f, 500.0f, "%.2f"))
		{
			startPosition = float2(startPos[0], startPos[1]); 
		}

		ImGui::EndChild();
		ImGui::PopStyleColor(); 

		ImGui::PopStyleColor();

		ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(2.0f, 2.0f));
		IMGUI_SPACED_SEPARATOR;

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
	}
}

void FollowPathAction::DrawBehaviorSettings()
{
	ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0.12f, 0.14f, 0.17f, 1.00f));
	int childSize = 110; 
	if (pathPlayMode == PATH_LOOP_TIMES)
	{
		childSize = 140;
	}

	if (isSpeedConstant)
		childSize += 30; 

	ImGui::PushFont(App->moduleImGui->rudaBlackBig);
	ImGui::TextColored(ImVec4(1, 1, 1, 1.0f), "Settings:");
	ImGui::PopFont();

	ImGui::BeginChild("##PathsSettingsHierarchy", ImVec2(ImGui::GetContentRegionAvailWidth(), childSize));

	INC_CURSOR_7;
	int pathModeCombo = (int)pathPlayMode;
	if (ImGui::Combo("Play Mode", &pathModeCombo, "Play Once\0Loop X Times\0Loop Teleport\0Loop Cycle\0"))
	{
		pathPlayMode = (PathPlayMode)pathModeCombo;
	}

	if (pathPlayMode == PATH_LOOP_TIMES)
	{
		INC_CURSOR_X_7;
		ImGui::InputInt("Times To Repeat", &targetLoopsAmount); 
	}

	INC_CURSOR_X_7;
	Texture* iconTexture = ResourceManager::getInstance()->GetTexture("PlayIcon");
	if (movementState == MOVEMENT_ONGOING || movementState == MOVEMENT_CYCLE)
	{
		iconTexture = ResourceManager::getInstance()->GetTexture("StopIcon");
	}
	else
	{
		iconTexture = ResourceManager::getInstance()->GetTexture("PlayIcon");
	}

	ImGui::Checkbox("Constant Speed", &isSpeedConstant);

	if (isSpeedConstant)
	{
		INC_CURSOR_X_7;
		if (ImGui::DragFloat("Constant Speed", &constantSpeed, 0.05, 0, 2))
		{
			SetConstantSpeed(constantSpeed);
		}
	}

	INC_CURSOR_X_7;
	if (ImGui::ImageButton((ImTextureID)iconTexture->GetTextureID(), ImVec2(30, 30)))
	{
		if(movementState == MOVEMENT_IDLE)
			DoAction(); 
		else
		{
			Stop(true); 
		}
	}

	ImGui::SameLine();
	ImGui::PushFont(App->moduleImGui->rudaBlackBig);
	ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 7);
	ImGui::TextColored(ImVec4(1, 1, 1, 1.0f), "Play Movement Preview");
	ImGui::PopFont();

	ImGui::EndChild();
	ImGui::PopStyleColor(); 

}

void FollowPathAction::SetConstantSpeed(float newConstSpeed)
{
	constantSpeed = newConstSpeed; 
	for (auto& currentStep : *pathSteps)
	{
		currentStep->SetMovementSpeed(constantSpeed);
	}
}

void FollowPathAction::DrawVisualSettings()
{
	INC_CURSOR_7;
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

void FollowPathAction::SaveAction(JSON_Object* jsonObject, string serializeObjectString, bool literalStr, int actionPositionInObject)
{
	string toolsSerializeSection;

	if (!literalStr)
		toolsSerializeSection = serializeObjectString + string("Actions.Action_") + to_string(actionPositionInObject) + ".";
	else
		toolsSerializeSection = serializeObjectString;

	Action::SaveAction(jsonObject, toolsSerializeSection);
	Action::SaveOccurrence(jsonObject, toolsSerializeSection);

	// Save Reproduce type 
	json_object_dotset_number(jsonObject, string(toolsSerializeSection + "PlayMode").c_str(), (int)pathPlayMode);
	json_object_dotset_boolean(jsonObject, string(toolsSerializeSection + "IsSpeedConstant").c_str(), isSpeedConstant);
	json_object_dotset_number(jsonObject, string(toolsSerializeSection + "ConstantSpeed").c_str(), constantSpeed);

	// Save Visuals 
	json_object_dotset_number(jsonObject, string(toolsSerializeSection + "Visuals.LineColor.r").c_str(), lineColor.x);
	json_object_dotset_number(jsonObject, string(toolsSerializeSection + "Visuals.LineColor.g").c_str(), lineColor.y);
	json_object_dotset_number(jsonObject, string(toolsSerializeSection + "Visuals.LineColor.b").c_str(), lineColor.z);
	json_object_dotset_number(jsonObject, string(toolsSerializeSection + "Visuals.LineColor.a").c_str(), lineColor.w);

	json_object_dotset_number(jsonObject, string(toolsSerializeSection + "Visuals.BoxColor.r").c_str(), graphBoxColor.x);
	json_object_dotset_number(jsonObject, string(toolsSerializeSection + "Visuals.BoxColor.g").c_str(), graphBoxColor.y);
	json_object_dotset_number(jsonObject, string(toolsSerializeSection + "Visuals.BoxColor.b").c_str(), graphBoxColor.z);
	json_object_dotset_number(jsonObject, string(toolsSerializeSection + "Visuals.BoxColor.a").c_str(), graphBoxColor.w);

	json_object_dotset_number(jsonObject, string(toolsSerializeSection + "Visuals.StartBoxColor.r").c_str(), startBoxColor.x);
	json_object_dotset_number(jsonObject, string(toolsSerializeSection + "Visuals.StartBoxColor.g").c_str(), startBoxColor.y);
	json_object_dotset_number(jsonObject, string(toolsSerializeSection + "Visuals.StartBoxColor.g").c_str(), startBoxColor.y);
	json_object_dotset_number(jsonObject, string(toolsSerializeSection + "Visuals.StartBoxColor.b").c_str(), startBoxColor.z);
	json_object_dotset_number(jsonObject, string(toolsSerializeSection + "Visuals.StartBoxColor.a").c_str(), startBoxColor.w);

	// Save Steps 
	string stepSerializeGroupStr = toolsSerializeSection + "PathSteps."; 
	json_object_dotset_number(jsonObject, string(stepSerializeGroupStr + "StepsAmount").c_str(), pathSteps->size());

	// Save Start Pos
	json_object_dotset_number(jsonObject, string(stepSerializeGroupStr + "StartPosition.x").c_str(), startPosition.x);
	json_object_dotset_number(jsonObject, string(stepSerializeGroupStr + "StartPosition.y").c_str(), startPosition.y);

	int count = 0; 
	for(auto& currentStep : *pathSteps)
	{
		string serializeStepStr = stepSerializeGroupStr + "Step_" + to_string(count) + ".";		
		currentStep->Save(jsonObject, serializeStepStr);
		count++; 
	}
}

void FollowPathAction::AddStep(PathStep* newStep, int stepIndex)
{
	if (newStep != nullptr)
	{
		if (pathSteps->empty())
			newStep->startPosition = parentObject->transform->GetPosition(true); 
		else
		{
			std::list<PathStep*>::iterator prevStep = pathSteps->begin();
			std::advance(prevStep, stepIndex - 1);
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
		float targetTime = pathSteps->front()->GetSpeed();
		pathSteps->front()->SetMovementSpeed(pathSteps->front()->GetSpeed());

		flyObjectInterpolation->SetInterpolationSegment(startPosition, finishPosition);
		flyObjectInterpolation->SetTargetTime(pathSteps->front()->GetTargetTime());	
	}

	if (pathPlayMode == PATH_LOOP_CYCLE || pathPlayMode == PATH_LOOP_TELEPORT)
	{
		SetActionCompleted(true); 
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
	(*currentStep)->SetMovementSpeed((*currentStep)->GetSpeed()); 

	flyObjectInterpolation->SetInterpolationSegment(startPosition, finishPosition);
	flyObjectInterpolation->SetTargetTime((*currentStep)->GetTargetTime()); 
	stepTime = 0;
}

void FollowPathAction::ResetPathMovement()
{
	currentStepIndex = 0; 
	stepTime = 0; 
	parentObject->transform->SetPosition(startPosition); 
	parentObject->FitObjectUtils(); 
}

PathPlayMode FollowPathAction::GetPathMode()
{
	return pathPlayMode;
}

void FollowPathAction::SetPathMode(PathPlayMode newPathMode)
{
	pathPlayMode = newPathMode;
}

float2 FollowPathAction::GetStartPosition()
{
	return startPosition;
}

void FollowPathAction::SetStartPosition(float2 _newStartPos)
{
	startPosition = _newStartPos;
}

float4 FollowPathAction::GetLineColor()
{
	return lineColor;
}

void FollowPathAction::SetLineColor(float4 newLineColor)
{
	lineColor = newLineColor;
}

float4 FollowPathAction::GetBoxColor()
{
	return graphBoxColor;
}

void FollowPathAction::SetBoxColor(float4 newBoxColor)
{
	graphBoxColor = newBoxColor;
}

float4 FollowPathAction::GetStartBoxColor()
{
	return startBoxColor;
}

void FollowPathAction::SetStartBoxColor(float4 newStartBoxColor)
{
	startBoxColor = newStartBoxColor; 
}

void FollowPathAction::SetIsSpeedConstant(bool isSpeedConst)
{
	isSpeedConstant = isSpeedConst; 
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

void PathStep::CleanUp()
{
	graphBox->CleanUp();
	delete graphBox; 
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

	if (ImGui::DragFloat("Speed", &speed, 0.05f, 0.5f, 10.0f))
	{
		SetMovementSpeed(speed);
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
	targetTime = length / (speed * 500);
}

float PathStep::GetSpeed()
{
	return speed;
}

float PathStep::GetTargetTime()
{
	return targetTime;
}
