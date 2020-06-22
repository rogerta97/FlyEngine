#include "FlyObject.h"
#include "RandomNumberGenerator.h"
#include "ModuleImGui.h"
#include "DisplayImageAction.h"
#include "Texture.h"
#include "TinyFileDialog.h"
#include "Quad.h"
#include "DialogueEditorDockPanel.h"
#include "DialogueNodeGraph.h"
#include "ImageImporter.h"
#include "DisplayAnimationAction.h"
#include "ActionConditionVariable.h"
#include "ActionConditionHasItem.h"
#include "ModuleManager.h"
#include "GameViewportDockPanel.h"
#include "FollowPathAction.h"
#include "DialogueAction.h"
#include "ChangeRoomAction.h"
#include "ModuleInput.h"
#include "DisplayTextAction.h"
#include "EmitSoundAction.h"
#include "ModuleWorldManager.h"
#include "GameInventory.h"
#include "ModifyVariableAction.h"
#include "ViewportManager.h"
#include "ResourceManager.h"
#include "imgui.h"
#include "MyFileSystem.h"
#include "Room.h"
#include "Gizmos.h"
#include <string>

#include "mmgr.h"

FlyObject::FlyObject(std::string _name, std::string _description, FlyObjectType _flyObjectType, Room* _parentRoom)
{
	name = _name;
	description = _description;
	uid = RandomNumberGenerator::GenerateUID();
	isSelected = false;
	hasVisuals = false;
	clickableAreaActive = false;
	isPicked = false;
	isInteractable = true;
	drawClickableArea = false;
	flyObjectType = _flyObjectType;
	parentRoom = _parentRoom;
	currentSequentialAction = nullptr;
	sequentialSwapedTick = false;

	clickableArea = new BoundingBox();
	clickableAreaPosPerc = float2(0, 0);
	clickableAreaSizePerc = float2(0, 0);
	clickableAreaColor = float4(1.0f, 1.0f, 1.0f, 0.4f);

	transform = new Transform();
	gizmos = new Gizmos(this);
}

FlyObject::FlyObject()
{

}

FlyObject::~FlyObject()
{

}

//Returns true if this objects is deleted in the middle of the 
bool FlyObject::Update(float dt)
{
	bool ret = false;

	if (isSelected)
	{
		if (!(selectedAction != nullptr && selectedAction->GetType() == ACTION_DISPLAY_TEXT))
			gizmos->Update();

		if (clickableArea != nullptr)
		{
			//clickableArea->Update();	
		}
	}

	for (auto& currentAction : actionsList)
	{
		currentAction->Update(dt);
	}

	for (auto& currentAction : sequentialActionsList)
	{
		currentAction->Update(dt);
	}

	// Update Sequential Action If Necessary ----------
	sequentialSwapedTick = false;
	if (currentSequentialAction != nullptr && !sequentialDoneOnce)
	{
		if (currentSequentialAction->IsActionFinished())
		{
			bool assignNext = false;
			bool assignedSuccess = false;
			int actionIndex = 0;
			for (auto currentSequential = sequentialActionsList.begin(); currentSequential != sequentialActionsList.end(); currentSequential++)
			{
				/*if (assignNext)
				{
					assignedSuccess = true;
					currentSequentialAction->StopAction();
					currentSequentialAction = currentSequential;
					break;
				}

				if (actionIndex == sequentialActionsList.size() - 1)
				{
					assignedSuccess = true;
					currentSequentialAction = nullptr;
					break;
				}*/

				if (*currentSequential == currentSequentialAction)
				{
					if (actionIndex < sequentialActionsList.size() - 1)
					{
						currentSequentialAction->StopAction();
						currentSequentialAction = *std::next(currentSequential);
						sequentialSwapedTick = true;
						break;
					}
					else
					{
						currentSequentialAction = nullptr;
						sequentialSwapedTick = true;
						sequentialDoneOnce = true; 
					}

				}

				actionIndex++;
			}

			/*		if (!assignedSuccess)
					{
						currentSequentialAction = nullptr;
					}*/
		}

		if (currentSequentialAction != nullptr)
		{
			currentSequentialAction->DoAction();
		}
	}

	// User Click Interaction -------------------------
	if (App->isEngineInPlayMode && App->moduleInput->GetMouseButton(RI_MOUSE_BUTTON_1_DOWN) == KEY_DOWN)
	{
		if (clickableArea->IsMouseOver())
		{
			DoOnMouseOverActions();
		}

		if (clickableArea->IsBoxClicked())
		{
			switch (flyObjectType)
			{
			case ACTION_OBJECT:
				DoOnClickActions();
				break;

			case INVENTORY_ITEM:
				DoOnClickActions();
				App->moduleWorldManager->GetSelectedRoom()->AddItemToInventory(this);
				isPicked = true;
				ret = true;
				break;

			case OBJECT_SEQUENTIAL:
				BeginSequentialActions();
				break;

			case OBJECT_CHARACTER:
				if (currentSequentialAction == nullptr && !sequentialActionsList.empty())
				{
					currentSequentialAction = sequentialActionsList.front();
					sequentialSwapedTick = true;
				}
				break;
			}
		}
	}

	// Sequential Start Conditions
	if (App->isEngineInPlayMode)
	{
		if (occ_ObjectClicked && clickableArea->IsBoxClicked())
		{
			if (currentSequentialAction == nullptr && !sequentialActionsList.empty())
			{
				flog("Start Sequential");
				currentSequentialAction = sequentialActionsList.front();
			}
		}
	}

	return ret;
}

void FlyObject::BeginSequentialActions()
{
	if (currentSequentialAction == nullptr && !sequentialActionsList.empty())
	{
		currentSequentialAction = sequentialActionsList.front();
	}

	sequentialSwapedTick = true;
}

void FlyObject::OnSceneEnter()
{
	for (auto& currentAction : actionsList)
	{
		if (currentAction->IsOccSceneEnter() && currentAction->PassAllOccurrenceConditions())
			currentAction->DoAction();
	}

	// Check Start Sequential 
	if (occ_SceneEnter && flyObjectType == FlyObjectType::OBJECT_SEQUENTIAL)
	{
		if (currentSequentialAction == nullptr && !sequentialActionsList.empty())
		{
			currentSequentialAction = sequentialActionsList.front();
		}

		sequentialSwapedTick = true;
	}
}

void FlyObject::OnSceneLeave()
{
	for (auto& currentAction : actionsList)
	{
		if (currentAction->IsOccSceneLeave())
			currentAction->DoAction();
	}
}

void FlyObject::DoOnMouseOverActions()
{
	for (auto& it : actionsList)
	{
		if (it->IsOccMouseOver())
		{
			if (it->IsOccCondition() && !it->PassConditionTest())
				continue;

			it->DoAction();
		}
	}
}

bool FlyObject::PassConditionTest()
{
	for (auto& currentCondition : sequentialActionConditions)
	{
		bool testResult = currentCondition->PassTestCondition();

		if (testResult == true)
		{
			if (sequentialEvaluationCriteria == SEQ_ONE_SUCCED)
				return true;
		}
		else
		{
			if (sequentialEvaluationCriteria == SEQ_ALL_SUCCED)
				return false;
		}
	}

	return true;
}

void FlyObject::DoVariableConditionActions(FlyVariable* currentVariableValue)
{
	for (auto& currentAction : actionsList)
	{
		if (App->moduleWorldManager->GetSelectedRoom() == this->parentRoom && currentAction->IsOccCondition() && currentAction->GetOccurrencesMarkedAmount() == 1 && currentAction->PassConditionTest())
		{
			currentAction->DoAction();
		}
	}

	if (App->moduleWorldManager->GetSelectedRoom() == this->parentRoom && occ_blackboardValue && PassConditionTest())
	{
		BeginSequentialActions(); 
	}
}

bool FlyObject::HasAction(ActionType _actionToCheckType)
{
	for (auto& currentAction : actionsList)
	{
		if (currentAction->GetType() == _actionToCheckType)
		{
			return true;
		}
	}

	return false;
}

bool FlyObject::HasSequentialAction(ActionType _actionToCheckType)
{
	for (auto& currentAction : sequentialActionsList)
	{
		if (currentAction->GetType() == _actionToCheckType)
		{
			return true;
		}
	}

	return false;
}

void FlyObject::DrawVisualLayer(bool forceDraw)
{
	if (isPicked && !forceDraw)
		return;

	for (auto& it : actionsList) {
		(it)->Draw();
	}

	for (auto& it : sequentialActionsList)
	{
		if (it->GetDrawIfSequential())
			(it)->Draw();
	}
}

void FlyObject::DrawDebugShapes()
{
	if (isSelected || ViewportManager::getInstance()->drawClickableAreaCondition == DRAW_ALWAYS)
	{
		if (clickableArea != nullptr && drawClickableArea && clickableAreaActive && !App->isEngineInPlayMode)
			DrawClickableArea();
	}

	for (auto& currentAction : actionsList)
	{
		currentAction->DrawDebugShapes(); 
	}

	for (auto& currentSequential : sequentialActionsList)
	{
		currentSequential->DrawDebugShapes();
	}

	if (isSelected)
	{
		if (selectedAction != nullptr && selectedAction->GetType() == ACTION_DISPLAY_TEXT)
			return;

		if(!App->isEngineInPlayMode)
			gizmos->Draw();
	}
}

void FlyObject::DrawClickableArea()
{
	if (drawClickableArea && clickableAreaActive)
	{
		clickableArea->Draw(true, clickableAreaColor);
	}
}

void FlyObject::CleanUp()
{
	for (auto& it : actionsList)
	{
		(it)->CleanUp();
		delete it;
	}

	for (auto& it : sequentialActionsList)
	{
		(it)->CleanUp();
		delete it;
	}

	actionsList.clear();
	delete transform;

	gizmos->CleanUp();
	delete gizmos;

	if (clickableArea != nullptr)
	{
		clickableArea->CleanUp();
		delete clickableArea;
	}

}

std::string FlyObject::GetName() const
{
	return name;
}

void FlyObject::SetName(std::string newName)
{
	name = newName;
}

UID FlyObject::GetUID() const
{
	return uid;
}

void FlyObject::SetUID(UID newUID)
{
	uid = newUID;
}

std::string FlyObject::GetDescription() const
{
	return description;
}

void FlyObject::SetDescription(std::string newDescription)
{
	description = newDescription;
}

float4 FlyObject::GetClickableAreaColor() const
{
	return clickableAreaColor;
}

void FlyObject::SetClickableAreaColor(float4 newColor)
{
	clickableAreaColor = newColor;
}

Room* FlyObject::GetParentRoom() const
{
	return parentRoom;
}

void FlyObject::SetParentRoom(Room* newParentRoom)
{
	if (newParentRoom != nullptr)
		parentRoom = newParentRoom;
}

bool& FlyObject::IsInteractable()
{
	return isInteractable;
}

void FlyObject::SetInteractable(bool _isInteractable)
{
	isInteractable = _isInteractable;
}

DisplayImageAction* FlyObject::GetDisplayImageAction()
{
	return (DisplayImageAction*)GetAction(ACTION_DISPLAY_IMAGE);
}

float2 FlyObject::GetObjectVisualDimensions()
{
	float2 objectVisualSize;

	// For now we will just take into account ImageTool, if you are deleting this you advanced really. 
	DisplayImageAction* imageTool = (DisplayImageAction*)GetAction(ACTION_DISPLAY_IMAGE);
	if (imageTool != nullptr)
	{
		objectVisualSize = float2(imageTool->GetQuad()->quadWidth, imageTool->GetQuad()->quadHeigth);
	}

	return objectVisualSize;
}

void FlyObject::CalculateAllGizmos()
{
	gizmos->CalculateSelectGizmo(this);
	gizmos->CalculateMoveGizmo(this);
	gizmos->CalculateScaleGizmo(this);
}

void FlyObject::FitObjectUtils()
{
	CalculateAllGizmos();
	if (clickableArea)
	{
		float2 offset = SetCASizeFromOne(clickableAreaPosPerc, clickableAreaSizePerc, !hasVisuals);
		clickableArea->SetPosition(float2(transform->GetPosition(true).x + offset.x, transform->GetPosition(true).y + offset.y));
	}
}

bool FlyObject::IsMouseOver()
{
	return gizmos->IsMouseOver();
}

void FlyObject::CopyFlyObjectActions(FlyObject* objectToClone)
{
	for (auto& currentOtherAction : objectToClone->actionsList)
	{
		switch (currentOtherAction->GetType())
		{
		case ACTION_DISPLAY_IMAGE:
		{
			DisplayImageAction* otherDisplayImageAction = (DisplayImageAction*)currentOtherAction;

			DisplayImageAction* newDisplayImage = AddDisplayImageAction();
			newDisplayImage->CreateImage(otherDisplayImageAction->GetTexture()->GetPath());
		}
		break;
		case ACTION_CHANGE_ROOM:
		{
			ChangeRoomAction* otherChangeRoomAction = (ChangeRoomAction*)currentOtherAction;

			ChangeRoomAction* changeRoomAction = AddChangeRoomAction();
			changeRoomAction->SetDestination(otherChangeRoomAction->GetDesination());

			break;
		}

		case ACTION_EMIT_SOUND:
		{
			EmitSoundAction* emitSoundAction = (EmitSoundAction*)currentOtherAction;

			EmitSoundAction* newEmitSoundAction = AddEmitSoundAction();
			newEmitSoundAction->audioClip = emitSoundAction->audioClip;
			newEmitSoundAction->audioPlayMode = emitSoundAction->audioPlayMode;

			break;
		}


		case ACTION_FOLLOW_PATH:
		{
			// No Working
			FollowPathAction* followPathAction = (FollowPathAction*)currentOtherAction;


			FollowPathAction* newFollowPathAction = new FollowPathAction(followPathAction);

			break;
		}

		case ACTION_DISPLAY_ANIMATION:
		{
			DisplayAnimationAction* otherDisplayAnimationAction = (DisplayAnimationAction*)currentOtherAction;

			DisplayAnimationAction* newDisplayAnimationAction = AddDisplayAnimationAction();


			break;
		}

		}
	}
}

bool FlyObject::HasVisuals()
{
	for (auto& currentTool : actionsList)
	{
		if (currentTool->HasVisual())
			return true;
	}

	return false;
}

bool FlyObject::IsInventoryItem()
{
	if (flyObjectType == INVENTORY_ITEM)
		return true;

	return false;
}

void FlyObject::SaveObjectData(JSON_Object* jsonObject, int objectIndex)
{
	// Save Object Properties
	string serializeObjectName = "RoomData.Objects.FlyObject_" + to_string(objectIndex) + string(".");

	json_object_dotset_string(jsonObject, string(serializeObjectName + "Name").c_str(), GetName().c_str());
	json_object_dotset_number(jsonObject, string(serializeObjectName + "UID").c_str(), uid);
	json_object_dotset_boolean(jsonObject, string(serializeObjectName + "Interactable").c_str(), isInteractable);
	json_object_dotset_number(jsonObject, string(serializeObjectName + "ObjectType").c_str(), this->flyObjectType);

	if (!GetDescription().empty())
		json_object_dotset_string(jsonObject, string(serializeObjectName + "Description").c_str(), GetDescription().c_str());

	SaveTransform(serializeObjectName, jsonObject);

	json_object_dotset_number(jsonObject, string(serializeObjectName + "Actions.ActionsAmount").c_str(), actionsList.size());


	// Save Object Action Settings
	int counter = 0;
	for (auto& it : actionsList)
	{
		it->SaveAction(jsonObject, serializeObjectName, false, counter);
		counter++;
	}

	json_object_dotset_number(jsonObject, string(serializeObjectName + "SequentialActions.ActionsAmount").c_str(), sequentialActionsList.size());

	// Save Sequential Occurrence 
	json_object_dotset_boolean(jsonObject, string(serializeObjectName + "SequentialActions.StartOccurrence.SceneEnter").c_str(), occ_SceneEnter);
	json_object_dotset_boolean(jsonObject, string(serializeObjectName + "SequentialActions.StartOccurrence.ObjectClicked").c_str(), occ_ObjectClicked);
	json_object_dotset_boolean(jsonObject, string(serializeObjectName + "SequentialActions.StartOccurrence.BlackboardCondition").c_str(), occ_blackboardValue);

	SaveSequentialConditions(string(serializeObjectName + "Sequential_BlackboardConditions."), jsonObject);

	//if (itemToClickWith == nullptr)
	//{
	//	json_object_dotset_number(jsonObject, string(serializeObjectName + "ItemToClickWith").c_str(), 0);
	//}
	//else
	//{
	//	json_object_dotset_number(jsonObject, string(serializeObjectName + "ItemToClickWith").c_str(), itemToClickWith->GetUID());
	//}

	// Save Sequential Actions
	counter = 0;
	for (auto& it : sequentialActionsList)
	{
		it->SaveAction(jsonObject, std::string(serializeObjectName + "SequentialActions."), false, counter);
		counter++;
	}

	// Save Object Clickable Area
	SaveClickableArea(serializeObjectName, jsonObject);
}

void FlyObject::SaveTransform(std::string serializeObjectName, JSON_Object* jsonObject)
{
	serializeObjectName += "Transform.";
	json_object_dotset_number(jsonObject, string(serializeObjectName + "Position.x").c_str(), transform->GetPosition().x);
	json_object_dotset_number(jsonObject, string(serializeObjectName + "Position.y").c_str(), transform->GetPosition().y);

	json_object_dotset_number(jsonObject, string(serializeObjectName + "Rotation.x").c_str(), transform->GetRotation().x);
	json_object_dotset_number(jsonObject, string(serializeObjectName + "Rotation.y").c_str(), transform->GetRotation().y);

	json_object_dotset_number(jsonObject, string(serializeObjectName + "Scale.x").c_str(), transform->GetScale().x);
	json_object_dotset_number(jsonObject, string(serializeObjectName + "Scale.y").c_str(), transform->GetScale().y);
}


void FlyObject::SaveSequentialConditions(std::string serializeObjectName, JSON_Object* jsonObject)
{
	int conditionsAmount = this->sequentialActionConditions.size();
	json_object_dotset_number(jsonObject, string(serializeObjectName + "Conditions.ConditionsAmount").c_str(), conditionsAmount);

	if (!sequentialActionConditions.empty())
	{
		string conditionsSaveStr = serializeObjectName + "Conditions.";
		int count = 0;
		for (auto& currentCondition : sequentialActionConditions)
		{
			currentCondition->SaveCondition(jsonObject, conditionsSaveStr, count++);
		}
	}
}

void FlyObject::SaveClickableArea(std::string serializeObjectName, JSON_Object* jsonObject)
{
	serializeObjectName += "ClickableArea.";

	json_object_dotset_boolean(jsonObject, string(serializeObjectName + "Active").c_str(), clickableAreaActive);
	json_object_dotset_boolean(jsonObject, string(serializeObjectName + "Draw").c_str(), drawClickableArea);
	json_object_dotset_boolean(jsonObject, string(serializeObjectName + "DirectPosition").c_str(), !hasVisuals);

	json_object_dotset_number(jsonObject, string(serializeObjectName + "Color.r").c_str(), clickableAreaColor.x);
	json_object_dotset_number(jsonObject, string(serializeObjectName + "Color.g").c_str(), clickableAreaColor.y);
	json_object_dotset_number(jsonObject, string(serializeObjectName + "Color.b").c_str(), clickableAreaColor.z);
	json_object_dotset_number(jsonObject, string(serializeObjectName + "Color.a").c_str(), clickableAreaColor.w);

	json_object_dotset_number(jsonObject, string(serializeObjectName + "MinPoint.x").c_str(), clickableArea->GetMinPoint().x);
	json_object_dotset_number(jsonObject, string(serializeObjectName + "MinPoint.y").c_str(), clickableArea->GetMinPoint().y);

	json_object_dotset_number(jsonObject, string(serializeObjectName + "MaxPoint.x").c_str(), clickableArea->GetMaxPoint().x);
	json_object_dotset_number(jsonObject, string(serializeObjectName + "MaxPoint.y").c_str(), clickableArea->GetMaxPoint().y);

	json_object_dotset_number(jsonObject, string(serializeObjectName + "PosPerc.x").c_str(), clickableAreaPosPerc.x);
	json_object_dotset_number(jsonObject, string(serializeObjectName + "PosPerc.y").c_str(), clickableAreaPosPerc.y);

	json_object_dotset_number(jsonObject, string(serializeObjectName + "SizePerc.x").c_str(), clickableAreaSizePerc.x);
	json_object_dotset_number(jsonObject, string(serializeObjectName + "SizePerc.y").c_str(), clickableAreaSizePerc.y);
}

void FlyObject::DoObjectActions()
{
}

void FlyObject::DoOnClickActions()
{
	for (auto& it : actionsList)
	{
		if (it->IsOccObjectClicked())
		{
			if (it->IsOccCondition() && !it->PassConditionTest())
				continue;

			if (it->itemToClickWith != nullptr)
			{
				if (GameInventory::getInstance()->droppingObject != it->itemToClickWith)
				{
					continue;
				}
				else
				{
					GameInventory::getInstance()->returnDroppingObject = true;
				}
			}
		}

		it->DoAction();
	}
}

void FlyObject::CalculateCurrentGizmo()
{
	switch (App->moduleImGui->gameViewportDockPanel->GetGizmoMode())
	{
	case GIZMO_SELECT:
		gizmos->CalculateSelectGizmo(this);
		break;

	case GIZMO_MOVE:
		gizmos->CalculateMoveGizmo(this);
		break;

	case GIZMO_SCALE:
		gizmos->CalculateScaleGizmo(this);
		break;
	}
}

DisplayImageAction* FlyObject::AddDisplayImageAction(const char* imageTexturePath, bool addToSequentialActions)
{
	if (addToSequentialActions)
	{
		// Add Action to the parent
	/*	if (!HasAction(ACTION_DISPLAY_IMAGE))
		{
			DisplayImageAction* displayImageAct = AddDisplayImageAction("Null", false);
			displayImageAct->SetVisible(false);
		}*/

		// Create Sequencial 
		DisplayImageAction* newAtrImage = new DisplayImageAction(this);
		newAtrImage->SetIsInfoHolder(true);
		newAtrImage->SetIsSequencial(true);
		newAtrImage->SetImageTextureByPath(imageTexturePath);

		sequentialActionsList.push_back(newAtrImage);
		return newAtrImage;
	}
	else if (GetAction(ACTION_DISPLAY_IMAGE) == nullptr)
	{
		if (imageTexturePath == "None")
			imageTexturePath = "EmptyObject";

		DisplayImageAction* newAtrImage = new DisplayImageAction(this);
		newAtrImage->CreateImage(imageTexturePath);

		if (addToSequentialActions)
			sequentialActionsList.push_back(newAtrImage);
		else
			actionsList.push_back(newAtrImage);

		// Addapt Gizmo Rect to new Image
		gizmos->FitSelectBoxSize();
		hasVisuals = true;

		return newAtrImage;
	}
	else if (imageTexturePath != "Null")
	{
		DisplayImageAction* newAtrImage = (DisplayImageAction*)GetAction(ACTION_DISPLAY_IMAGE);

		string textureName = imageTexturePath;
		textureName = MyFileSystem::getInstance()->GetLastPathItem(imageTexturePath, false);

		Texture* newTexture = (Texture*)ResourceManager::GetResource(textureName);

		if (newTexture == nullptr)
			newTexture = (Texture*)ResourceManager::GetResource("EmptyObject");

		newAtrImage->SetTexture(newTexture);
	}


	return (DisplayImageAction*)GetAction(ACTION_DISPLAY_IMAGE);
}

ChangeRoomAction* FlyObject::AddChangeRoomAction(bool addToSequentialActions)
{
	if (GetAction(ACTION_CHANGE_ROOM) == nullptr)
	{
		ChangeRoomAction* changeSceneTool = new ChangeRoomAction(this);

		if (addToSequentialActions)
			sequentialActionsList.push_back(changeSceneTool);
		else
			actionsList.push_back(changeSceneTool);

		gizmos->FitSelectBoxSize();

		return changeSceneTool;
	}
	else if (addToSequentialActions)
	{
		ChangeRoomAction* changeSceneTool = new ChangeRoomAction(this);

		sequentialActionsList.push_back(changeSceneTool);
	}

	return (ChangeRoomAction*)GetAction(ACTION_CHANGE_ROOM);
}

ModifyVariableAction* FlyObject::AddModifyVariableAction(bool addToSequentialActions)
{
	if (GetAction(ACTION_MOD_VARIABLE) == nullptr)
	{
		ModifyVariableAction* mofidyVarAction = new ModifyVariableAction(this);
		if (addToSequentialActions)
			sequentialActionsList.push_back(mofidyVarAction);
		else
			actionsList.push_back(mofidyVarAction);
		return mofidyVarAction;
	}
	else if (addToSequentialActions)
	{
		ModifyVariableAction* mofidyVarAction = new ModifyVariableAction(this);


		sequentialActionsList.push_back(mofidyVarAction);
	}

	return (ModifyVariableAction*)GetAction(ACTION_MOD_VARIABLE);
}

EmitSoundAction* FlyObject::AddEmitSoundAction(bool addToSequentialActions)
{
	if (GetAction(ACTION_EMIT_SOUND) == nullptr)
	{
		EmitSoundAction* emitSoundAction = new EmitSoundAction(this);
		if (addToSequentialActions)
			sequentialActionsList.push_back(emitSoundAction);
		else
			actionsList.push_back(emitSoundAction);

		return emitSoundAction;
	}
	else if (addToSequentialActions)
	{
		EmitSoundAction* emitSoundAction = new EmitSoundAction(this);


		sequentialActionsList.push_back(emitSoundAction);

		return emitSoundAction;
	}

	return (EmitSoundAction*)GetAction(ACTION_EMIT_SOUND);
}

ActionCondition* FlyObject::AddEmptySequentialCondition(ActionConditionType conditionType)
{
	switch (conditionType)
	{
	case CONDITION_IS_VARIABLE:
	{
		ActionConditionVariable* conditionCheckVar = new ActionConditionVariable();
		sequentialActionConditions.push_back(conditionCheckVar);
		return conditionCheckVar;
	}
	case CONDITION_HAS_ITEM:
	{
		ActionConditionHasItem* conditionHasItem = new ActionConditionHasItem();
		sequentialActionConditions.push_back(conditionHasItem);
		return conditionHasItem;
	}
	case AC_NONE:
		break;
	}

	return nullptr;
}

DisplayAnimationAction* FlyObject::AddDisplayAnimationAction(bool addToSequentialActions)
{
	if (addToSequentialActions)
	{
		DisplayAnimationAction* newAtrAnimation = new DisplayAnimationAction(this);
		newAtrAnimation->SetIsInfoHolder(true);
		newAtrAnimation->SetIsSequencial(true);

		sequentialActionsList.push_back(newAtrAnimation);

		DisplayImageAction* fixedImageAction = (DisplayImageAction*)GetAction(ACTION_DISPLAY_IMAGE);

		// If there is a fixed image
		if (fixedImageAction != nullptr)
		{
			newAtrAnimation->AttachToImage(fixedImageAction);
		}

		//// Add Action to the parent
		//if (!HasAction(ACTION_DISPLAY_ANIMATION))
		//{
		//	DisplayAnimationAction* displayAnimAct = AddDisplayAnimationAction(false);
		//	displayAnimAct->SetVisible(false);
		//}

		// Get Fixed Image

		// Create Sequencial 

		return newAtrAnimation;
	}
	else if (GetAction(ACTION_DISPLAY_ANIMATION) == nullptr)
	{
		DisplayAnimationAction* displayAnimationAction = new DisplayAnimationAction(this);
		displayAnimationAction->SetIsSequencial(false);

		DisplayImageAction* fixedImageAction = (DisplayImageAction*)GetAction(ACTION_DISPLAY_IMAGE);

		if (fixedImageAction == nullptr)
		{
			fixedImageAction = AddDisplayImageAction(std::string(MyFileSystem::getInstance()->GetIconsDirectory() + "EmptyObject.png").c_str(), false);
		}

		fixedImageAction->fromAnimation = true;

		// If there is a fixed image
		if (fixedImageAction != nullptr)
		{
			displayAnimationAction->AttachToImage(fixedImageAction);
		}

		if (addToSequentialActions)
			sequentialActionsList.push_back(displayAnimationAction);
		else
			actionsList.push_back(displayAnimationAction);

		return displayAnimationAction;
	}

	return (DisplayAnimationAction*)GetAction(ACTION_DISPLAY_ANIMATION);
}

FollowPathAction* FlyObject::AddFollowPathAction(bool addToSequentialActions)
{
	if (GetAction(ACTION_FOLLOW_PATH) == nullptr)
	{
		FollowPathAction* followPathAction = new FollowPathAction(this);
		if (addToSequentialActions)
			sequentialActionsList.push_back(followPathAction);
		else
			actionsList.push_back(followPathAction);
		return followPathAction;
	}
	else if (addToSequentialActions)
	{
		FollowPathAction* followPathAction = new FollowPathAction(this);
		sequentialActionsList.push_back(followPathAction);
		return followPathAction;
	}

	return (FollowPathAction*)GetAction(ACTION_FOLLOW_PATH);
}

DialogueAction* FlyObject::AddDialogueAction(bool addToSequentialActions)
{
	if (GetAction(ACTION_DIALOGUE) == nullptr)
	{
		DialogueAction* dialogueAction = new DialogueAction(this);

		if (addToSequentialActions)
			sequentialActionsList.push_back(dialogueAction);
		else
			actionsList.push_back(dialogueAction);

		return dialogueAction;
	}
	else if (addToSequentialActions)
	{
		DialogueAction* dialogueAction = new DialogueAction(this);
		sequentialActionsList.push_back(dialogueAction);
		return dialogueAction;
	}

	return (DialogueAction*)GetAction(ACTION_DIALOGUE);
}

void FlyObject::DrawSequentialActionsList()
{
	ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0.12f, 0.14f, 0.17f, 1.00f));
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(5, 5));

	ImGui::BeginChild("##AttributesChildSequential", ImVec2(ImGui::GetContentRegionAvailWidth(), 200));

	int pos = 0;
	for (auto& currentAction : GetSequentialActionsList())
	{
		ActionSelectableInfo selectableInfo = currentAction->GetActionSelectableInfo();

		if (currentAction->GetType() == ACTION_DISPLAY_IMAGE)
		{
			DisplayImageAction* displayImageAction = (DisplayImageAction*)currentAction;

			if (displayImageAction->fromAnimation == true)
				continue;
		}

		ImGui::PushFont(App->moduleImGui->rudaBoldMid);

		Texture* imageIcon = App->moduleManager->GetIconFromActionType(selectableInfo.actionType);
		int selectableHeight = 42;

		ImGui::SetCursorPos(ImVec2(10, 5 + (selectableHeight * pos)));
		ImGui::Image((ImTextureID)imageIcon->GetTextureID(), ImVec2(30, 30), ImVec2(0, 1), ImVec2(1, 0));

		ImGui::SetCursorPos(ImVec2(50, (selectableHeight * pos) + 4));
		if (ImGui::Selectable(std::string(selectableInfo.actionName + "##" + to_string(currentAction->GetUID())).c_str(), &currentAction->IsSelected(), ImGuiSelectableFlags_None, ImVec2(ImGui::GetContentRegionMax().x, selectableHeight - 3)))
		{
			SetSelectedAction(currentAction->GetUID(), currentAction->IsActionSequential());
			selectedAction = currentAction;
		}
		ImGui::PopFont();

		// Description -----
		ImGui::SetCursorPosY((selectableHeight * pos) + 24);
		ImGui::SetCursorPosX(ImGui::GetCursorPos().x + 52);

		ImGui::PushFont(App->moduleImGui->rudaRegularTiny);
		ImGui::TextWrapped(selectableInfo.actionDescription.c_str());
		ImGui::PopFont();

		pos++;
	}

	ImGui::EndChild();

	ImGui::PopStyleVar();
	ImGui::PopStyleColor();
}

DisplayTextAction* FlyObject::AddDisplayTextAction(bool addToSequentialActions)
{
	if (GetAction(ACTION_DISPLAY_TEXT) == nullptr)
	{
		DisplayTextAction* displayTextAction = new DisplayTextAction(this);
		displayTextAction->SetText("Text");

		if (addToSequentialActions)
			sequentialActionsList.push_back(displayTextAction);
		else
			actionsList.push_back(displayTextAction);

		// Addapt Gizmo Rect to new Text
		//gizmos->FitSelectBoxSize();

		return displayTextAction;
	}
	else if (addToSequentialActions)
	{
		DisplayTextAction* displayTextAction = new DisplayTextAction(this);

		sequentialActionsList.push_back(displayTextAction);

		return displayTextAction;
	}

	return (DisplayTextAction*)GetAction(ACTION_DISPLAY_TEXT);
}

void FlyObject::SetSelectedAction(UID toolTypeSelectedUID, bool isSequential)
{
	bool prevSelectedDialogue = false;

	if (selectedAction != nullptr && selectedAction->GetType() == ACTION_DIALOGUE)
		prevSelectedDialogue = true;

	for (auto& it : actionsList)
	{
		if (it->GetUID() == toolTypeSelectedUID && !isSequential)
		{
			it->SetSelected(true);
			selectedAction = it;
		}
		else
			it->SetSelected(false);
	}

	for (auto& it : sequentialActionsList)
	{
		if (it->GetUID() == toolTypeSelectedUID && isSequential)
		{
			it->SetSelected(true);
			selectedAction = it;
		}
		else
			it->SetSelected(false);
	}

	if (selectedAction != nullptr)
	{
		DialogueAction* selectedDialogueAction = (DialogueAction*)selectedAction;

		if (prevSelectedDialogue)
			App->moduleImGui->dialogueEditorDockPanel->GetNodeGraph()->saveData = true;

		if (selectedAction->GetType() == ACTION_DIALOGUE)
			App->moduleImGui->dialogueEditorDockPanel->GetNodeGraph()->AttachDialogue(selectedDialogueAction->GetDialogueData());
	}
}

ActionType FlyObject::GetSelectedActionType()
{
	if (selectedAction == nullptr)
		return AT_null;

	return selectedAction->GetType();
}

Action* FlyObject::GetAction(std::string toolName)
{
	for (auto& currentAction : actionsList)
	{
		if (currentAction->GetActionName() == toolName)
			return currentAction;
	}

	return nullptr;
}

Action* FlyObject::GetAction(ActionType toolType)
{
	for (auto& currentAction : actionsList)
	{
		if (currentAction->GetType() == toolType)
			return currentAction;
	}

	return nullptr;
}

Action* FlyObject::GetAction(UID actionUID)
{
	for (auto& currentAction : actionsList)
	{
		if (currentAction->GetUID() == actionUID)
			return currentAction;
	}

	return nullptr;
}

Action* FlyObject::GetSequentialAction(ActionType toolType)
{
	for (auto& currentAction : sequentialActionsList)
	{
		if (currentAction->GetType() == toolType)
			return currentAction;
	}

	return nullptr;
}

std::list<Action*> FlyObject::GetActionsList() const
{
	return actionsList;
}

std::list<Action*> FlyObject::GetSequentialActionsList() const
{
	return sequentialActionsList;
}

void FlyObject::DeleteAction(ActionType actionType)
{
	for (auto currentTool = actionsList.begin(); currentTool != actionsList.end(); currentTool++)
	{
		if ((*currentTool)->GetType() == actionType)
		{
			(*currentTool)->CleanUp();
			delete (*currentTool);
			(*currentTool) = nullptr;
			currentTool = actionsList.erase(currentTool);
			return;
		}
	}
}

void FlyObject::DeleteSequentialAction(UID actionUID)
{
	for (auto currentTool = sequentialActionsList.begin(); currentTool != sequentialActionsList.end(); currentTool++)
	{
		if ((*currentTool)->GetUID() == actionUID)
		{
			(*currentTool)->CleanUp();
			delete (*currentTool);
			(*currentTool) = nullptr;
			currentTool = sequentialActionsList.erase(currentTool);
			return;
		}
	}
}

void FlyObject::DeleteAction(UID actionUID)
{
	for (auto currentTool = actionsList.begin(); currentTool != actionsList.end(); currentTool++)
	{
		if ((*currentTool)->GetUID() == actionUID)
		{
			(*currentTool)->CleanUp();
			delete (*currentTool);
			(*currentTool) = nullptr;
			currentTool = actionsList.erase(currentTool);
			return;
		}
	}
}

// 0 == up / 1 == down
void FlyObject::MoveSequentialAction(UID actionUID, int moveType)
{
	int counter = 0;
	for (auto currentSequential = sequentialActionsList.begin(); currentSequential != sequentialActionsList.end(); currentSequential++)
	{
		if ((*currentSequential)->GetUID() == actionUID)
		{
			if (moveType == 0 && counter > 0)
			{
				std::swap(*currentSequential, *std::prev(currentSequential));
			}

			if (moveType == 1 && counter < sequentialActionsList.size() - 1)
			{
				std::swap(*currentSequential, *std::next(currentSequential));
			}
		}
		counter++;
	}
}

BoundingBox* FlyObject::GetClickableArea()
{
	return clickableArea;
}

void FlyObject::CreateClickableArea(float2 percentagePos, float2 percentageSize, bool directPosition)
{
	if (clickableArea == nullptr)
		clickableArea = new BoundingBox();

	SetCASizeFromOne(percentagePos, percentageSize, directPosition);

	clickableAreaPosPerc = percentagePos;
	clickableAreaSizePerc = percentageSize;
}


float2& FlyObject::GetClickableAreaPosOne()
{
	return clickableAreaPosPerc;
}

float2& FlyObject::GetClickableAreaSizeOne()
{
	return clickableAreaSizePerc;
}

void FlyObject::SetClickableAreaPosOne(float2 newAreaPosOne)
{
	clickableAreaPosPerc = newAreaPosOne;
}

void FlyObject::SetClickableAreaSizeOne(float2 newAreaSizeOne)
{
	clickableAreaSizePerc = newAreaSizeOne;
}

Action* FlyObject::GetSelectedAction()
{
	return selectedAction;
}

float2 FlyObject::SetCASizeFromOne(float2 percentagePos, float2 percentageSize, bool directiPosition)
{
	if (clickableArea == nullptr)
		return float2::zero;

	float2 offsetFromCenter;

	if (directiPosition)
	{
		clickableArea->SetMinPoint(float2(-percentageSize.x, percentageSize.y));
		clickableArea->SetMaxPoint(float2(percentageSize.x, -percentageSize.y));
		offsetFromCenter = float2::zero;
	}
	else
	{
		float2 objectSize = GetObjectVisualDimensions();

		float2 objectTopLeft = float2(-objectSize.x / 2, -objectSize.y / 2);
		float2 clickable_area_size = float2(objectSize.x * percentageSize.x, objectSize.y * percentageSize.y);
		float2 clickable_area_pos = float2(objectSize.x * percentagePos.x, objectSize.y * percentagePos.y);

		clickableArea->SetMinPoint(float2(objectTopLeft.x + clickable_area_pos.x, objectTopLeft.y + clickable_area_pos.y + clickable_area_size.y));
		clickableArea->SetMaxPoint(float2(objectTopLeft.x + clickable_area_pos.x + clickable_area_size.x, objectTopLeft.y + clickable_area_pos.y));

		clickableArea->SetMinPoint(float2(clickableArea->GetMinPoint().x * transform->GetScale().x, clickableArea->GetMinPoint().y * transform->GetScale().y));
		clickableArea->SetMaxPoint(float2(clickableArea->GetMaxPoint().x * transform->GetScale().x, clickableArea->GetMaxPoint().y * transform->GetScale().y));

		this->clickableAreaPosPerc = percentagePos;
		this->clickableAreaSizePerc = percentageSize;

		float2 objectCenter = objectTopLeft + GetObjectVisualDimensions() / 2;
		offsetFromCenter = clickableArea->GetCenter() - objectCenter;
	}

	return offsetFromCenter;
}

void FlyObject::DrawDisplayImageSettings()
{
	DisplayImageAction* imageTool = (DisplayImageAction*)GetAction("Image");
	Texture* imageTexture = imageTool->GetTexture();

	if (ImGui::CollapsingHeader("Image Adjustments"))
	{
		static char buf[256] = "";
		if (imageTexture == nullptr)
			imageTexture = (Texture*)ResourceManager::getInstance()->GetResource("EmptyObject");
		else
			imageTexture = (Texture*)ResourceManager::getInstance()->GetResource(imageTool->GetTexture()->GetName());

		float aspect_ratio = imageTexture->GetAspectRatio();
		float previewQuadWidth = 150;
		float previewQuadHeight = previewQuadWidth / aspect_ratio;

		ImGui::Spacing();
		PUSH_FONT(App->moduleImGui->rudaRegularMid);

		ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0.12f, 0.14f, 0.17f, 1.00f));
		ImGui::BeginChild("##4ShowImage", ImVec2(ImGui::GetContentRegionAvailWidth(), previewQuadHeight + 18));

		ImGui::Columns(2);
		ImGui::SetColumnWidth(0, previewQuadWidth + 10);

		ImGui::Spacing();
		ImGui::Image((ImTextureID)imageTexture->GetTextureID(), ImVec2(previewQuadWidth, previewQuadHeight));

		ImGui::NextColumn();

		ImGui::Spacing();
		ImGui::Text("Name: "); ImGui::SameLine();
		ImGui::TextColored(ImVec4(0.1f, 0.7f, 1.0f, 1.0f), "%s", imageTexture->GetName().c_str());

		ImGui::Text("Width: "); ImGui::SameLine();
		ImGui::TextColored(ImVec4(0.1f, 0.7f, 1.0f, 1.0f), "%d", imageTexture->GetWidth());

		ImGui::Text("Height: "); ImGui::SameLine();
		ImGui::TextColored(ImVec4(0.1f, 0.7f, 1.0f, 1.0f), "%d", imageTexture->GetHeigth());

		Texture* searchTexture = (Texture*)ResourceManager::getInstance()->GetResource("SearchIcon");
		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));

		if (ImGui::ImageButton((ImTextureID)searchTexture->GetTextureID(), ImVec2(24, 24)))
		{
			char const* lFilterPatterns[2] = { "*.jpg" , "*.png" };
			const char* path = tinyfd_openFileDialog("Load Image...", NULL, 2, lFilterPatterns, NULL, 0);

			if (path != NULL)
			{
				if (!ResourceManager::getInstance()->ExistResourcePath(path))
				{
					imageTexture = ImageImporter::getInstance()->LoadTexture(path, false);
					ResourceManager::getInstance()->AddResource(imageTexture, imageTexture->GetName());
				}
				else
				{
					imageTexture = (Texture*)ResourceManager::getInstance()->GetResourceByPath(path);
				}

				strcpy(buf, path);
				flog("Player Opened %s", path);
			}

		}

		ImGui::PopStyleColor(2);
		ImGui::EndChild();
		ImGui::PopFont();
	}
}

//void FlyObject::/*DrawToolsList*/()
//{
//	ImGui::PushFont(App->moduleImGui->rudaBoldBig);
//	ImGui::Text("Add Object Actions: ");
//	ImGui::PopFont();
//
//	ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0.12f, 0.14f, 0.17f, 1.00f));
//	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(5, 5));
//
//	ImGui::BeginChild("##AttributesChild", ImVec2(ImGui::GetContentRegionAvailWidth(), 200));
//
//	if (showImageTool)
//	{
//		ToolSelectableInfo toolInfo = App->moduleManager->GetToolNameDescription("Image");
//		if (DrawToolSelectable(toolInfo)) selectedInList = AT_IMAGE;
//	}
//
//	ImGui::EndChild();
//
//	ImGui::PopStyleVar();
//	ImGui::PopStyleColor();
//}
