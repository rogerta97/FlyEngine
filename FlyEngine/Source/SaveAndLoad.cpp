#include "SaveAndLoad.h"

#include "MyFileSystem.h"
#include "Application.h"
#include "ModuleWorldManager.h"
#include "StepAnswer.h"
#include "ModifyVariableAction.h"
#include "DisplayAnimationAction.h"
#include "FollowPathAction.h"
#include "DialogueAction.h"
#include "ImageImporter.h"
#include "ResourceManager.h"
#include "GameViewportDockPanel.h"
#include "FlyObjectCharacter.h"
#include "ModuleImGui.h"
#include "RoomUIHandler.h"
#include "GamePropertiesDockPanel.h"
#include "Animation.h"
#include "Room.h"
#include "DialogueStep.h"
#include "Dialogue.h"
#include "FlyObject.h"

#include "EmitSoundAction.h"
#include "Gizmos.h"
#include "DisplayImageAction.h"
#include "DisplayTextAction.h"
#include "ChangeRoomAction.h"

#include <fstream>

#include "mmgr.h"

SaveAndLoad* SaveAndLoad::instance = 0;

SaveAndLoad* SaveAndLoad::getInstance()
{
	if (instance == 0)
		instance = new SaveAndLoad();

	return instance;
}

SaveAndLoad::~SaveAndLoad()
{

}

void SaveAndLoad::SaveCurrentRoomData()
{
	Room* currentRoom = App->moduleWorldManager->GetSelectedRoom(); 
	instance->SaveRoomData(currentRoom); 
}

void SaveAndLoad::SaveRoomData(std::string roomName)
{
	instance->SaveRoomData(App->moduleWorldManager->GetRoom(roomName));
}

void SaveAndLoad::SaveRoomData(UID roomUID)
{
	instance->SaveRoomData(App->moduleWorldManager->GetRoom(roomUID));
}

void SaveAndLoad::SaveRoomData(Room* roomToSave)
{
	if (roomToSave == nullptr)
		return; 

	std::string roomName = roomToSave->GetName(); 
	std::string saveFilePath = MyFileSystem::getInstance()->GetSavedDataDirectory() + "\\RoomsData\\" + roomName.c_str() + ".json"; 

	JSON_Value* scene_v = json_value_init_object();
	JSON_Object* scene_obj = json_value_get_object(scene_v);

	roomToSave->SaveRoomData(scene_obj);

	json_serialize_to_file(scene_v, saveFilePath.c_str());

	App->moduleWorldManager->SaveConfigData();
}

void SaveAndLoad::SaveSelectedRoomToOnPlayData()
{
	Room* selectedRoom = App->moduleWorldManager->GetSelectedRoom(); 

	string provisionalPath = MyFileSystem::getInstance()->GetSavedDataDirectory() + "OnPlaySaveData.json"; 

	JSON_Value* scene_v = json_value_init_object();
	JSON_Object* scene_obj = json_value_get_object(scene_v);

	selectedRoom->SaveRoomData(scene_obj);

	json_serialize_to_file(scene_v, provisionalPath.c_str());
}

void SaveAndLoad::LoadOnPlayData()
{
	string provisionalPath = MyFileSystem::getInstance()->GetSavedDataDirectory() + "OnPlaySaveData.json";

	JSON_Value* root = json_parse_file(provisionalPath.c_str());
	JSON_Object* root_obj = json_value_get_object(root);

	UID selectedWhenPlayRoomUID = json_object_dotget_number(root_obj, "RoomData.UID"); 
	Room* nextSelectedRoom = App->moduleWorldManager->GetRoom(selectedWhenPlayRoomUID); 
	App->moduleWorldManager->SetSelectedRoom(nextSelectedRoom); 

	App->moduleWorldManager->GetSelectedRoom()->CleanUp(); 

	instance->LoadDataToSelectedRoom(provisionalPath);
}

void SaveAndLoad::SaveInitFile()
{
	if (App->IsReleaseVersion())
		return; 

	Room* startRoom = App->moduleWorldManager->GetStartRoom();

	string savePath = MyFileSystem::getInstance()->GetSavedDataDirectory() + "Init.json";

	JSON_Value* scene_v = json_value_init_object();
	JSON_Object* scene_obj = json_value_get_object(scene_v);
	
	if(startRoom != nullptr)
		json_object_dotset_number(scene_obj, "InitData.StartRoomUID", startRoom->GetUID()); 
	else
		json_object_dotset_number(scene_obj, "InitData.StartRoomUID", 0); 

	string projectName = App->moduleImGui->gamePropertiesDockPanel->GetProjectName(); 
	json_object_dotset_string(scene_obj, "InitData.ProjectName", projectName.c_str());

	//if(!projectName.empty())
	//	json_object_dotset_string(scene_obj, "InitData.ProjectName", App->moduleImGui->gamePropertiesDockPanel->GetProjectName().c_str());
	//else

	json_object_dotset_boolean(scene_obj, "InitData.GameMode", App->GetGameMode());

	json_serialize_to_file(scene_v, savePath.c_str());

	// Set Hidden File
	//int attr = GetFileAttributes(savePath.c_str());
	//if ((attr & FILE_ATTRIBUTE_HIDDEN) == 0)
	//{
	//	SetFileAttributes(savePath.c_str(), attr | FILE_ATTRIBUTE_HIDDEN);
	//}
}

void SaveAndLoad::LoadDataToSelectedRoom(std::string roomDataFilePath)
{
	// Get Current Room 
	Room* currentRoom = App->moduleWorldManager->GetSelectedRoom();
	LoadDataToRoom(roomDataFilePath, currentRoom); 
}

void SaveAndLoad::CreateFlyObjectFromSavedData(JSON_Object* root_obj, std::string& serializeObjectStr, Room* currentRoom)
{
	// Object Data -------
	string newObjectName = json_object_dotget_string(root_obj, string(serializeObjectStr + string("Name")).c_str());
	int flyObjectType = json_object_dotget_number(root_obj, string(serializeObjectStr + string("ObjectType")).c_str());
	
	// Create Object --
	FlyObject* newObject = nullptr; 

	if (flyObjectType == INVENTORY_ITEM)	
		newObject = currentRoom->CreateInventoryItem(newObjectName.c_str());	
	else if(flyObjectType == ACTION_OBJECT)
		newObject = currentRoom->CreateFlyObject(newObjectName.c_str());
	else if (flyObjectType == OBJECT_SEQUENTIAL)
		newObject = currentRoom->CreateFlyObjectSequencial(newObjectName.c_str());
	else if (flyObjectType == OBJECT_CHARACTER)
		newObject = currentRoom->CreateCharacter(newObjectName.c_str(), "", true);
	
	// UID --
	newObject->SetUID(json_object_dotget_number(root_obj, string(serializeObjectStr + string("UID")).c_str())); 

	if (json_object_dothas_value(root_obj, string(serializeObjectStr + string("Description")).c_str())) 
	{
		newObject->SetDescription(json_object_dotget_string(root_obj, string(serializeObjectStr + string("Description")).c_str()));
	}

	// Interactable
	newObject->SetInteractable(json_object_dotget_boolean(root_obj, string(serializeObjectStr + string("Interactable")).c_str()));

	// Parent Room --
	newObject->SetParentRoom(currentRoom); 

	// Transform ------------
	float positionX = json_object_dotget_number(root_obj, string(serializeObjectStr + string("Transform.Position.x")).c_str()); 
	float positionY = json_object_dotget_number(root_obj, string(serializeObjectStr + string("Transform.Position.y")).c_str()); 
	float2 position(positionX, positionY); 
	newObject->transform->SetPosition(position); 

	float rotationX = json_object_dotget_number(root_obj, string(serializeObjectStr + string("Transform.Rotation.x")).c_str());
	float rotationY = json_object_dotget_number(root_obj, string(serializeObjectStr + string("Transform.Rotation.y")).c_str());
	float2 rotation(rotationX, rotationY);
	newObject->transform->SetRotationEuler(rotation);

	float scaleX = json_object_dotget_number(root_obj, string(serializeObjectStr + string("Transform.Scale.x")).c_str());
	float scaleY = json_object_dotget_number(root_obj, string(serializeObjectStr + string("Transform.Scale.y")).c_str());
	float2 scale(scaleX, scaleY);
	newObject->transform->SetScale(scale);

	// Load Animations in case the object is a character 
	if (newObject->flyObjectType == OBJECT_CHARACTER)
	{
		FlyObjectCharacter* characterFO = (FlyObjectCharacter*)newObject; 
		characterFO->InitCharacter(); 

		// Load Idle Animation ---------------------------------------------------------------------------
		characterFO->GetIdleAnimation()->SetIsSequencial(false);
		characterFO->GetIdleAnimation()->SetActionClass((ActionClass)ActionClass::ACTION_CLASS_DIRECT);

		string idleAnimStr = serializeObjectStr + "CharacterAnimations.IdleAnimation.";
		int idleFramesAmount = json_object_dotget_number(root_obj, string(idleAnimStr + "FramesAmount").c_str());

		if (idleFramesAmount > 0)
		{
			int playModeTmp = json_object_dotget_number(root_obj, string(idleFramesAmount + "PlayMode").c_str());
			characterFO->GetIdleAnimation()->animPlayMode = (AnimationPlayMode)playModeTmp;

			characterFO->GetIdleAnimation()->GetAnimation()->SetSpeed(json_object_dotget_number(root_obj, string(idleAnimStr + "Speed").c_str()));

			int count = 0;
			while (count < idleFramesAmount)
			{
				string frameStr = idleAnimStr + "Frames.Frame_" + to_string(count);

				string textureName = json_object_dotget_string(root_obj, string(frameStr + ".TextureName").c_str());
				Texture* frameTexture = (Texture*)ResourceManager::getInstance()->GetResource(textureName);

				if (frameTexture)
					characterFO->GetIdleAnimation()->AddFrame(frameTexture);

				count++;
			}
		}

		// Load Walk Animation ---------------------------------------------------------------------------
		characterFO->GetWalkAnimation()->SetIsSequencial(false);
		characterFO->GetWalkAnimation()->SetActionClass((ActionClass)ActionClass::ACTION_CLASS_DIRECT);

		string walkAnimStr = serializeObjectStr + "CharacterAnimations.WalkAnimation.";
		int walkFramesAmount = json_object_dotget_number(root_obj, string(walkAnimStr + "FramesAmount").c_str());

		if (walkFramesAmount > 0)
		{
			int playModeTmp = json_object_dotget_number(root_obj, string(walkFramesAmount + "PlayMode").c_str());
			characterFO->GetWalkAnimation()->animPlayMode = (AnimationPlayMode)playModeTmp;

			characterFO->GetWalkAnimation()->GetAnimation()->SetSpeed(json_object_dotget_number(root_obj, string(walkAnimStr + "Speed").c_str()));

			int count = 0;
			while (count < walkFramesAmount)
			{
				string frameStr = walkAnimStr + "Frames.Frame_" + to_string(count);

				string textureName = json_object_dotget_string(root_obj, string(frameStr + ".TextureName").c_str());
				Texture* frameTexture = (Texture*)ResourceManager::getInstance()->GetResource(textureName);

				if (frameTexture)
					characterFO->GetWalkAnimation()->AddFrame(frameTexture);

				count++;
			}
		}

		// Load Talk Animation ---------------------------------------------------------------------------
		characterFO->GetTalkAnimation()->SetIsSequencial(false);
		characterFO->GetTalkAnimation()->SetActionClass((ActionClass)ActionClass::ACTION_CLASS_DIRECT);

		string talkAnimStr = serializeObjectStr + "CharacterAnimations.TalkAnimation.";
		int talkFramesAmount = json_object_dotget_number(root_obj, string(talkAnimStr + "FramesAmount").c_str());

		if (talkFramesAmount > 0)
		{
			int playModeTmp = json_object_dotget_number(root_obj, string(talkFramesAmount + "PlayMode").c_str());
			characterFO->GetTalkAnimation()->animPlayMode = (AnimationPlayMode)playModeTmp;

			characterFO->GetTalkAnimation()->GetAnimation()->SetSpeed(json_object_dotget_number(root_obj, string(talkAnimStr + "Speed").c_str()));

			int count = 0;
			while (count < talkFramesAmount)
			{
				string frameStr = talkAnimStr + "Frames.Frame_" + to_string(count);

				string textureName = json_object_dotget_string(root_obj, string(frameStr + ".TextureName").c_str());
				Texture* frameTexture = (Texture*)ResourceManager::getInstance()->GetResource(textureName);

				if (frameTexture)
					characterFO->GetTalkAnimation()->AddFrame(frameTexture);

				count++;
			}
		}
	}

	// Create Actions -------
	if (json_object_dothas_value(root_obj, string(serializeObjectStr + string("Actions")).c_str()))
	{
		string serializeObjectStrActions = serializeObjectStr + "Actions.";
		int actionsAmount = json_object_dotget_number(root_obj, string(serializeObjectStrActions + string("ActionsAmount")).c_str());

		int counter = 0;
		while (counter < actionsAmount)
		{
			std::string actionSectionStr = serializeObjectStrActions + "Action_" + to_string(counter) + ".";
			int objectType = json_object_dotget_number(root_obj, string(actionSectionStr + string("ActionType")).c_str());
			ActionType actionType = (ActionType)objectType;

			switch (actionType)
			{
			case ACTION_DISPLAY_IMAGE:
				instance->LoadDisplayImageAction(root_obj, actionSectionStr, newObject);
				break;
			case ACTION_CHANGE_ROOM:
				instance->LoadChangeRoomAction(root_obj, actionSectionStr, newObject);
				break;
			case ACTION_MOD_VARIABLE:
				instance->LoadModifyVariableAction(root_obj, actionSectionStr, newObject, currentRoom);
				break;
			case ACTION_EMIT_SOUND:
				instance->LoadEmitSoundAction(root_obj, actionSectionStr, newObject);
				break;
			case ACTION_DISPLAY_TEXT:
				instance->LoadDisplayTextAction(root_obj, actionSectionStr, newObject);
				break;
			case ACTION_DISPLAY_ANIMATION:
				instance->LoadDisplayAnimationAction(root_obj, actionSectionStr, newObject);
				break;
			case ACTION_FOLLOW_PATH:
				instance->LoadFollowPathAction(root_obj, actionSectionStr, newObject);
				break;
			case ACTION_DIALOGUE:
				instance->LoadDialogueAction(root_obj, actionSectionStr, newObject);
				break;
			case AT_null:
				break;
			default:
				break;
			}
			counter++;
		}
	}

	// Create Sequential Actions ----
	if (json_object_dothas_value(root_obj, string(serializeObjectStr + string("SequentialActions")).c_str()))
	{
		string serializeObjectStrActions = serializeObjectStr + "SequentialActions.";
		int actionsAmount = json_object_dotget_number(root_obj, string(serializeObjectStrActions + string("ActionsAmount")).c_str());

		int counter = 0;
		while (counter < actionsAmount)
		{
			std::string actionSectionStr = serializeObjectStrActions + "Actions.Action_" + to_string(counter) + ".";
			int objectType = json_object_dotget_number(root_obj, string(actionSectionStr + string("ActionType")).c_str());
			ActionType actionType = (ActionType)objectType;

			switch (actionType)
			{
			case ACTION_DISPLAY_IMAGE:
				instance->LoadDisplayImageAction(root_obj, actionSectionStr, newObject);
				break;
			case ACTION_CHANGE_ROOM:
				instance->LoadChangeRoomAction(root_obj, actionSectionStr, newObject);
				break;
			case ACTION_MOD_VARIABLE:
				instance->LoadModifyVariableAction(root_obj, actionSectionStr, newObject, currentRoom);
				break;
			case ACTION_EMIT_SOUND:
				instance->LoadEmitSoundAction(root_obj, actionSectionStr, newObject);
				break;
			case ACTION_DISPLAY_TEXT:
				instance->LoadDisplayTextAction(root_obj, actionSectionStr, newObject);
				break;
			case ACTION_DISPLAY_ANIMATION:
				instance->LoadDisplayAnimationAction(root_obj, actionSectionStr, newObject);
				break;
			case ACTION_FOLLOW_PATH:
				instance->LoadFollowPathAction(root_obj, actionSectionStr, newObject);
				break;
			case ACTION_DIALOGUE:
				instance->LoadDialogueAction(root_obj, actionSectionStr, newObject);
				break;
			case AT_null:
				break;
			default:
				break;
			}
			counter++;
		}
	}

	// Clickable Area
	newObject->clickableAreaActive = json_object_dotget_boolean(root_obj, string(serializeObjectStr + string("ClickableArea.Active")).c_str());
	newObject->drawClickableArea = json_object_dotget_boolean(root_obj, string(serializeObjectStr + string("ClickableArea.Draw")).c_str());
	bool directPosition = json_object_dotget_boolean(root_obj, string(serializeObjectStr + "ClickableArea.DirectPosition").c_str());

	float4 color; 
	color.x = json_object_dotget_number(root_obj, string(serializeObjectStr + string("ClickableArea.Color.r")).c_str());
	color.y = json_object_dotget_number(root_obj, string(serializeObjectStr + string("ClickableArea.Color.g")).c_str());
	color.z = json_object_dotget_number(root_obj, string(serializeObjectStr + string("ClickableArea.Color.b")).c_str());
	color.w = json_object_dotget_number(root_obj, string(serializeObjectStr + string("ClickableArea.Color.w")).c_str());

	float minPointX = json_object_dotget_number(root_obj, string(serializeObjectStr + string("ClickableArea.MinPoint.x")).c_str());
	float minPointY = json_object_dotget_number(root_obj, string(serializeObjectStr + string("ClickableArea.MinPoint.y")).c_str());

	float maxPointX = json_object_dotget_number(root_obj, string(serializeObjectStr + string("ClickableArea.MaxPoint.x")).c_str());
	float maxPointY = json_object_dotget_number(root_obj, string(serializeObjectStr + string("ClickableArea.MaxPoint.y")).c_str());

	float posPercX = json_object_dotget_number(root_obj, string(serializeObjectStr + "ClickableArea.PosPerc.x").c_str());
	float posPercY = json_object_dotget_number(root_obj, string(serializeObjectStr + "ClickableArea.PosPerc.y").c_str());

	float sizePercX = json_object_dotget_number(root_obj, string(serializeObjectStr + "ClickableArea.SizePerc.x").c_str());
	float sizePercY = json_object_dotget_number(root_obj, string(serializeObjectStr + "ClickableArea.SizePerc.y").c_str());

	newObject->CreateClickableArea(float2(posPercX, posPercY), float2(sizePercX, sizePercY), directPosition); 
 
	App->moduleImGui->gameViewportDockPanel->FitViewportToRegion();
	newObject->FitObjectUtils();
}

void SaveAndLoad::LoadDialogueAction(JSON_Object* root_obj, std::string& serializeObjectStrActions, FlyObject* newObject)
{
	int actionClass = json_object_dotget_number(root_obj, string(serializeObjectStrActions + "ActionClass").c_str());
	DialogueAction* dialogueAction = nullptr;

	if (actionClass == ACTION_CLASS_SEQUENTIAL && (newObject->flyObjectType == FlyObjectType::OBJECT_SEQUENTIAL || newObject->flyObjectType == FlyObjectType::OBJECT_CHARACTER))
		dialogueAction = newObject->AddDialogueAction(true);
	else
		dialogueAction = newObject->AddDialogueAction();

	dialogueAction->SetActionClass((ActionClass)actionClass);
	dialogueAction->LoadOccurrence(root_obj, serializeObjectStrActions + string("Occurrence."));

	// Load Dialogue Data 
	UID nodeGraphID = json_object_dotget_number(root_obj, string(serializeObjectStrActions + "NodeGraphID").c_str());

	if (nodeGraphID != 0)
		dialogueAction->GetDialogueData()->needReload = true;

	// Load Steps Data
	string serialiseStepsStr = serializeObjectStrActions + "Steps.";
	int stepsAmount = json_object_dotget_number(root_obj, string(serialiseStepsStr + "StepsAmount").c_str());

	for (int y = 0; y < stepsAmount; y++)
	{
		// Load Name & Text 
		string stepSerializeStr = serialiseStepsStr + "Step_" + to_string(y) + ".";

		DialogueStep* newStep = dialogueAction->AddDialogueStep();

		string stepName = json_object_dotget_string(root_obj, string(stepSerializeStr + "Name").c_str());
		string stepText = json_object_dotget_string(root_obj, string(stepSerializeStr + "Text").c_str());
		UID stepUID = json_object_dotget_number(root_obj, string(stepSerializeStr + "StepID").c_str());
		bool isFirst = json_object_dotget_boolean(root_obj, string(stepSerializeStr + "First").c_str());

		// Load Step Data
		newStep->SetName(stepName);
		newStep->SetText(stepText);
		newStep->SetUID(stepUID);

		newStep->isFirst = isFirst;

		// Visuals For Sentence 
		newStep->fontNameHold = json_object_dotget_string(root_obj, string(stepSerializeStr + "Visuals.FontName").c_str());

		newStep->backgroundColorHold.x = json_object_dotget_number(root_obj, string(stepSerializeStr + "Visuals.BackgroundColor.r").c_str());
		newStep->backgroundColorHold.y = json_object_dotget_number(root_obj, string(stepSerializeStr + "Visuals.BackgroundColor.g").c_str());
		newStep->backgroundColorHold.z = json_object_dotget_number(root_obj, string(stepSerializeStr + "Visuals.BackgroundColor.b").c_str());
		newStep->backgroundColorHold.w = json_object_dotget_number(root_obj, string(stepSerializeStr + "Visuals.BackgroundColor.a").c_str());

		newStep->fontColorHold.x = json_object_dotget_number(root_obj, string(stepSerializeStr + "Visuals.FontColor.r").c_str());
		newStep->fontColorHold.y = json_object_dotget_number(root_obj, string(stepSerializeStr + "Visuals.FontColor.g").c_str());
		newStep->fontColorHold.z = json_object_dotget_number(root_obj, string(stepSerializeStr + "Visuals.FontColor.b").c_str());
		newStep->fontColorHold.w = json_object_dotget_number(root_obj, string(stepSerializeStr + "Visuals.FontColor.a").c_str());

		// Visuals for answers 
		newStep->answerFontNameHold = json_object_dotget_string(root_obj, string(stepSerializeStr + "Answers.Visuals.FontName").c_str());

		newStep->answerBackgroundColorHold.x = json_object_dotget_number(root_obj, string(stepSerializeStr + "Answers.Visuals.BackgroundColor.r").c_str());
		newStep->answerBackgroundColorHold.y = json_object_dotget_number(root_obj, string(stepSerializeStr + "Answers.Visuals.BackgroundColor.g").c_str());
		newStep->answerBackgroundColorHold.z = json_object_dotget_number(root_obj, string(stepSerializeStr + "Answers.Visuals.BackgroundColor.b").c_str());
		newStep->answerBackgroundColorHold.w = json_object_dotget_number(root_obj, string(stepSerializeStr + "Answers.Visuals.BackgroundColor.a").c_str());

		newStep->answerFontColorHold.x = json_object_dotget_number(root_obj, string(stepSerializeStr + "Answers.Visuals.FontColor.r").c_str());
		newStep->answerFontColorHold.y = json_object_dotget_number(root_obj, string(stepSerializeStr + "Answers.Visuals.FontColor.g").c_str());
		newStep->answerFontColorHold.z = json_object_dotget_number(root_obj, string(stepSerializeStr + "Answers.Visuals.FontColor.b").c_str());
		newStep->answerFontColorHold.w = json_object_dotget_number(root_obj, string(stepSerializeStr + "Answers.Visuals.FontColor.a").c_str());

		dialogueAction->GetDialogueData()->stepsMap.insert(std::make_pair(newStep->GetUID(), newStep));
	}

	int count = 0;
	for (auto& currentStep : dialogueAction->GetDialogueData()->GetDialogueSteps())
	{
		// Load Step Answers ------------------
		string answersSerializeStr = serialiseStepsStr + "Step_" + to_string(count) + "." + "Answers.";

		int answersAmount = json_object_dotget_number(root_obj, string(answersSerializeStr + "AnswersAmount").c_str());

		for (int k = 0; k < answersAmount; k++)
		{
			// Load Name & Text 
			string answerSerializeStr = answersSerializeStr + "Answer_" + to_string(k) + ".";

			string answerName = json_object_dotget_string(root_obj, string(answerSerializeStr + "Name").c_str());
			string answerText = json_object_dotget_string(root_obj, string(answerSerializeStr + "Text").c_str());
			UID answerUID = json_object_dotget_number(root_obj, string(answerSerializeStr + "AnswerUID").c_str());

			UID linkUID = json_object_dotget_number(root_obj, string(answerSerializeStr + "Link.linkUID").c_str());
			UID startPinUID = json_object_dotget_number(root_obj, string(answerSerializeStr + "Link.startPinUID").c_str());
			UID endPinUID = json_object_dotget_number(root_obj, string(answerSerializeStr + "Link.endPinUID").c_str());

			StepAnswer* createdAnswer = currentStep->AddStepAnswer(answerText, answerName);

			createdAnswer->SetName(answerName);
			createdAnswer->SetAnswerText(answerText);

			// Load answers UIDs ------------------------------------------------------------------
			createdAnswer->SetUID(answerUID);

			// Load Link --------------------------------------------------------------------------
			createdAnswer->GetLink()->startPinUID = startPinUID;
			createdAnswer->GetLink()->endPinUID = endPinUID;
			createdAnswer->GetLink()->linkUID = linkUID;

			dialogueAction->GetDialogueData()->answersMap.insert(std::make_pair(createdAnswer->GetUID(), createdAnswer));

			// Answer Destination Step	-----------------------------------------------------------	
			UID destinationStepUID = json_object_dotget_number(root_obj, string(answerSerializeStr + "DestinationStepUID").c_str());

			if (destinationStepUID != 0)
			{
				DialogueStep* dstDialogueStep = dialogueAction->GetDialogueData()->GetStepFromID(destinationStepUID);
				createdAnswer->GetLink()->destinationStep = dstDialogueStep;
			}

			// Answer Callback Actions ------------------------------------------------------------
			bool hasCallbackActions = json_object_dotget_boolean(root_obj, string(answerSerializeStr + "CallbackActions.HasCallbackActions").c_str());

			if (hasCallbackActions)
			{
				int effectsAmount = json_object_dotget_number(root_obj, string(answerSerializeStr + string("CallbackActions.EffectsAmount")).c_str());
				ModifyVariableAction* newCallbackModifyAction = new ModifyVariableAction(nullptr);

				string effectsGroupStr = answerSerializeStr + "CallbackActions.EffectsGroup.";
				int count = 0;
				while (count < effectsAmount)
				{
					ModifyVariableEffect* newEffect = new ModifyVariableEffect();

					string effectSerializeStr = effectsGroupStr + "Effect_" + to_string(count) + ".";

					// Load Target Variable -----------
					string varName = json_object_dotget_string(root_obj, string(effectSerializeStr + string("TargetVariableName")).c_str());

					FlyVariable* targetVariable = newObject->GetParentRoom()->GetBlackboardVariable(varName);
					newEffect->targetVariable = targetVariable;

					// Load Operator Type, IncInt, NextToggle ----------
					int varOpTmp = (int)json_object_dotget_number(root_obj, string(effectSerializeStr + string("OperatorType")).c_str());
					newEffect->variableOperatorType = (VariableOperatorType)varOpTmp;

					newEffect->incIntegerValue = json_object_dotget_number(root_obj, string(effectSerializeStr + string("IncIntegerValue")).c_str());
					newEffect->nextToggleValue = json_object_dotget_number(root_obj, string(effectSerializeStr + string("NextToggleValue")).c_str());

					newCallbackModifyAction->AddEffect(newEffect);
					count++;
				}

				createdAnswer->SetModifyVarEffect(newCallbackModifyAction);
			}

		}

		count++;
	}
}

void SaveAndLoad::LoadFollowPathAction(JSON_Object* root_obj, std::string& serializeObjectStrActions, FlyObject* newObject)
{
	int actionClass = json_object_dotget_number(root_obj, string(serializeObjectStrActions + "ActionClass").c_str());
	FollowPathAction* followPathAction = nullptr;

	if (actionClass == ACTION_CLASS_SEQUENTIAL && (newObject->flyObjectType == FlyObjectType::OBJECT_SEQUENTIAL || newObject->flyObjectType == FlyObjectType::OBJECT_CHARACTER))
		followPathAction = newObject->AddFollowPathAction(true);
	else
		followPathAction = newObject->AddFollowPathAction();

	followPathAction->SetActionClass((ActionClass)actionClass);
	followPathAction->LoadOccurrence(root_obj, serializeObjectStrActions + string("Occurrence."));

	// Load Reproduce type 
	int playModeTmp = json_object_dotget_number(root_obj, string(serializeObjectStrActions + "PlayMode").c_str());
	followPathAction->SetPathMode((PathPlayMode)playModeTmp);

	bool isSpeedConstant = json_object_dotget_boolean(root_obj, string(serializeObjectStrActions + "IsSpeedConstant").c_str());
	followPathAction->SetIsSpeedConstant(isSpeedConstant);

	if (isSpeedConstant)
	{
		float constantSpeed = json_object_dotget_number(root_obj, string(serializeObjectStrActions + "ConstantSpeed").c_str());
		followPathAction->SetConstantSpeed(constantSpeed);
	}

	// Load Visuals 
	float4 lineColor = float4::zero; 
	lineColor.x = json_object_dotget_number(root_obj, string(serializeObjectStrActions + "Visuals.LineColor.r").c_str());
	lineColor.y = json_object_dotget_number(root_obj, string(serializeObjectStrActions + "Visuals.LineColor.g").c_str());
	lineColor.z = json_object_dotget_number(root_obj, string(serializeObjectStrActions + "Visuals.LineColor.b").c_str());
	lineColor.w = json_object_dotget_number(root_obj, string(serializeObjectStrActions + "Visuals.LineColor.a").c_str());
	followPathAction->SetLineColor(lineColor); 

	float4 boxColor = float4::zero;
	boxColor.x = json_object_dotget_number(root_obj, string(serializeObjectStrActions + "Visuals.BoxColor.r").c_str());
	boxColor.y = json_object_dotget_number(root_obj, string(serializeObjectStrActions + "Visuals.BoxColor.g").c_str());
	boxColor.z = json_object_dotget_number(root_obj, string(serializeObjectStrActions + "Visuals.BoxColor.b").c_str());
	boxColor.w = json_object_dotget_number(root_obj, string(serializeObjectStrActions + "Visuals.BoxColor.a").c_str());
	followPathAction->SetBoxColor(boxColor);

	float4 startBoxColor = float4::zero;
	startBoxColor.x = json_object_dotget_number(root_obj, string(serializeObjectStrActions + "Visuals.StartBoxColor.r").c_str());
	startBoxColor.y = json_object_dotget_number(root_obj, string(serializeObjectStrActions + "Visuals.StartBoxColor.g").c_str());
	startBoxColor.z = json_object_dotget_number(root_obj, string(serializeObjectStrActions + "Visuals.StartBoxColor.b").c_str());
	startBoxColor.w = json_object_dotget_number(root_obj, string(serializeObjectStrActions + "Visuals.StartBoxColor.a").c_str());
	//followPathAction->SetStartBoxColor(startBoxColor);

	// Load Path Steps 
	int stepsAmount = json_object_dotget_number(root_obj, string(serializeObjectStrActions + "PathSteps.StepsAmount").c_str());

	// Save Start Pos
	float2 startPos = float2::zero;
	startPos.x = json_object_dotget_number(root_obj, string(serializeObjectStrActions + "PathSteps.StartPosition.x").c_str());
	startPos.y = json_object_dotget_number(root_obj, string(serializeObjectStrActions + "PathSteps.StartPosition.y").c_str());
	followPathAction->SetStartPosition(startPos); 

	int count = 0;
	while (count < stepsAmount)
	{
		string stepStr = serializeObjectStrActions + "PathSteps.Step_" + to_string(count) + ".";

		PathStep* newStep = new PathStep();
		newStep->targetPosition.x = json_object_dotget_number(root_obj, string(stepStr + "TargetPosition.x").c_str());
		newStep->targetPosition.y = json_object_dotget_number(root_obj, string(stepStr + "TargetPosition.y").c_str());
		newStep->SetMovementSpeed(json_object_dotget_number(root_obj, string(stepStr + "MovementSpeed").c_str()));

		followPathAction->AddStep(newStep, count);

		count++;
	}
}

void SaveAndLoad::LoadDisplayAnimationAction(JSON_Object* root_obj, std::string& serializeObjectStrActions, FlyObject* newObject)
{
	int actionClass = json_object_dotget_number(root_obj, string(serializeObjectStrActions + "ActionClass").c_str());

	DisplayAnimationAction* displayAnimationAction = nullptr;
	if (actionClass == ACTION_CLASS_SEQUENTIAL)
		displayAnimationAction = newObject->AddDisplayAnimationAction(true);
	else
		displayAnimationAction = newObject->AddDisplayAnimationAction();

	displayAnimationAction->SetActionClass((ActionClass)actionClass);
	displayAnimationAction->LoadOccurrence(root_obj, serializeObjectStrActions + string("Occurrence."));

	int framesAmount = json_object_dotget_number(root_obj, string(serializeObjectStrActions + "FramesAmount").c_str());


	if (framesAmount > 0)
	{
		int playModeTmp = json_object_dotget_number(root_obj, string(serializeObjectStrActions + "PlayMode").c_str());
		displayAnimationAction->animPlayMode = (AnimationPlayMode)playModeTmp;

		displayAnimationAction->GetAnimation()->SetSpeed(json_object_dotget_number(root_obj, string(serializeObjectStrActions + "Speed").c_str()));

		int count = 0;
		while (count < framesAmount)
		{
			string frameStr = serializeObjectStrActions + "Frames.Frame_" + to_string(count);

			string textureName = json_object_dotget_string(root_obj, string(frameStr + ".TextureName").c_str());
			Texture* frameTexture = (Texture*)ResourceManager::getInstance()->GetResource(textureName);

			if (frameTexture)
				displayAnimationAction->AddFrame(frameTexture);

			count++;
		}
	}
}

void SaveAndLoad::LoadDisplayTextAction(JSON_Object* root_obj, std::string& serializeObjectStrActions, FlyObject* newObject)
{
	int actionClass = json_object_dotget_number(root_obj, string(serializeObjectStrActions + "ActionClass").c_str());

	DisplayTextAction* displayTextAction = nullptr;
	if (actionClass == ACTION_CLASS_SEQUENTIAL)
		displayTextAction = newObject->AddDisplayTextAction(true);
	else
		displayTextAction = newObject->AddDisplayTextAction();

	displayTextAction->SetActionClass((ActionClass)actionClass);
	displayTextAction->LoadOccurrence(root_obj, serializeObjectStrActions + string("Occurrence."));

	float2 minPoint = float2::zero; 
	float2 maxPoint = float2::zero; 

	minPoint.x = json_object_dotget_number(root_obj, string(serializeObjectStrActions + string("TextBox.MinPoint.x")).c_str());
	minPoint.y = json_object_dotget_number(root_obj, string(serializeObjectStrActions + string("TextBox.MinPoint.y")).c_str());

	maxPoint.x = json_object_dotget_number(root_obj, string(serializeObjectStrActions + string("TextBox.MaxPoint.x")).c_str());
	maxPoint.y = json_object_dotget_number(root_obj, string(serializeObjectStrActions + string("TextBox.MaxPoint.y")).c_str());

	displayTextAction->GetTextBox()->SetMinPoint(minPoint);
	displayTextAction->GetTextBox()->SetMaxPoint(maxPoint);

	string fontTmp = json_object_dotget_string(root_obj, string(serializeObjectStrActions + string("FontName")).c_str());
	if (fontTmp != "None")
	{
		Font* textFont = (Font*)ResourceManager::GetResource(fontTmp.c_str(), RESOURCE_FONT);

		if (textFont)
			displayTextAction->SetFont(textFont);
	}

	string textTmp = json_object_dotget_string(root_obj, string(serializeObjectStrActions + string("Text")).c_str());
	if (textTmp != "None")
	{
		displayTextAction->SetText(textTmp);
	}

	displayTextAction->GetTextBox()->CalculateAllGizmos();
}

void SaveAndLoad::LoadEmitSoundAction(JSON_Object* root_obj, std::string& serializeObjectStrActions, FlyObject* newObject)
{
	string audioClipPath = json_object_dotget_string(root_obj, string(serializeObjectStrActions + string("Path")).c_str());
	int actionClass = json_object_dotget_number(root_obj, string(serializeObjectStrActions + "ActionClass").c_str());

	EmitSoundAction* emitSoundAction = nullptr;
	if (actionClass == ACTION_CLASS_SEQUENTIAL)
		emitSoundAction = newObject->AddEmitSoundAction(true);
	else
		emitSoundAction = newObject->AddEmitSoundAction();

	emitSoundAction->SetActionClass((ActionClass)actionClass);
	emitSoundAction->SetActionClass((ActionClass)actionClass); 
	emitSoundAction->LoadOccurrence(root_obj, serializeObjectStrActions + string("Occurrence."));

	if (audioClipPath != "None")
	{
		string audioClipResourceName = MyFileSystem::getInstance()->GetLastPathItem(audioClipPath, false);
		AudioClip* soundClipResource = (AudioClip*)ResourceManager::getInstance()->GetResource(audioClipResourceName.c_str());
		emitSoundAction->audioClip = soundClipResource;
	}
}

void SaveAndLoad::LoadModifyVariableAction(JSON_Object* root_obj, std::string& serializeObjectStrActions, FlyObject* newObject, Room* currentRoom)
{
	int effectsAmount = json_object_dotget_number(root_obj, string(serializeObjectStrActions + string("ModifyVariable.EffectsAmount")).c_str());

	int actionClass = json_object_dotget_number(root_obj, string(serializeObjectStrActions + "DisplayImage.ActionClass").c_str());

	ModifyVariableAction* modifyVariableAction = nullptr;
	if (actionClass == ACTION_CLASS_SEQUENTIAL)
		modifyVariableAction = newObject->AddModifyVariableAction(true);
	else
		modifyVariableAction = newObject->AddModifyVariableAction();

	modifyVariableAction->SetActionClass((ActionClass)actionClass);
	modifyVariableAction->LoadOccurrence(root_obj, serializeObjectStrActions + string("ModifyVariable.Occurrence."));

	string effectsGroupStr = serializeObjectStrActions + "ModifyVariable.EffectsGroup.";
	int count = 0;
	while (count < effectsAmount)
	{
		ModifyVariableEffect* newEffect = new ModifyVariableEffect();

		string effectSerializeStr = effectsGroupStr + "Effect_" + to_string(count) + ".";

		// Load Target Variable -----------
		string varName = json_object_dotget_string(root_obj, string(effectSerializeStr + string("TargetVariableName")).c_str());

		FlyVariable* targetVariable = currentRoom->GetBlackboardVariable(varName);
		newEffect->targetVariable = targetVariable;

		// Load Operator Type, IncInt, NextToggle ----------
		int varOpTmp = (int)json_object_dotget_number(root_obj, string(serializeObjectStrActions + string("ModifyVariable.OperatorType")).c_str());
		newEffect->variableOperatorType = (VariableOperatorType)varOpTmp;

		newEffect->incIntegerValue = json_object_dotget_number(root_obj, string(serializeObjectStrActions + string("ModifyVariable.IncIntegerValue")).c_str());
		newEffect->nextToggleValue = json_object_dotget_number(root_obj, string(serializeObjectStrActions + string("ModifyVariable.NextToggleValue")).c_str());

		modifyVariableAction->AddEffect(newEffect);
		count++;
	}
}

void SaveAndLoad::LoadChangeRoomAction(JSON_Object* root_obj, std::string& serializeObjectStrActions, FlyObject* newObject)
{
	UID destinationRoomUID = json_object_dotget_number(root_obj, string(serializeObjectStrActions + string("Destination")).c_str());

	int actionClass = json_object_dotget_number(root_obj, string(serializeObjectStrActions + "ActionClass").c_str());

	ChangeRoomAction* changeRoomAction = nullptr;
	if (actionClass == ACTION_CLASS_SEQUENTIAL)
		changeRoomAction = newObject->AddChangeRoomAction(true);
	else
		changeRoomAction = newObject->AddChangeRoomAction();

	changeRoomAction->SetActionClass((ActionClass)actionClass);
	changeRoomAction->LoadOccurrence(root_obj, serializeObjectStrActions + string("Occurrence."));

	Room* room = App->moduleWorldManager->GetRoom(destinationRoomUID);
	changeRoomAction->SetDestination(room);

	if(room != nullptr)
		changeRoomAction->GetParentObject()->GetParentRoom()->AddConnection(room);
}

void SaveAndLoad::LoadDisplayImageAction(JSON_Object* root_obj, std::string& serializeObjectStrActions, FlyObject* newObject)
{
	string textureName = json_object_dotget_string(root_obj, string(serializeObjectStrActions + string("TextureName")).c_str());
	int actionClass = json_object_dotget_number(root_obj, string(serializeObjectStrActions + "ActionClass").c_str());

	DisplayImageAction* displayImageAction = nullptr;
	if (actionClass == ACTION_CLASS_SEQUENTIAL)
		displayImageAction = newObject->AddDisplayImageAction(textureName.c_str(), true);
	else
		displayImageAction = newObject->AddDisplayImageAction(textureName.c_str());

	displayImageAction->SetActionClass((ActionClass)actionClass);
	displayImageAction->LoadOccurrence(root_obj, serializeObjectStrActions + string("Occurrence."));
}

void SaveAndLoad::LoadActionConditions(JSON_Object* root_obj, std::string& serializeObjectStr, Room* currentRoom)
{
	int objectUID = json_object_dotget_number(root_obj, string(serializeObjectStr + "UID").c_str());
	FlyObject* parentObject = currentRoom->GetFlyObject(objectUID); 

	if (parentObject == nullptr)
	{
		FLY_ERROR("Parent Object could not be found when loading action conditions");
		assert(false);
	}
	
	if (json_object_dothas_value(root_obj, string(serializeObjectStr + string("Actions")).c_str()))
	{	
		string actionStr = serializeObjectStr + "Actions"; 
		int actionsAmount = json_object_dotget_number(root_obj, string(actionStr + ".ActionsAmount").c_str());

		for (int i = 0; i < actionsAmount; i++)
		{
			string serializeObjectStrActions = actionStr + ".Action_" + to_string(i);
			UID itemToClickWithUID = json_object_dotget_number(root_obj, string(serializeObjectStrActions + ".Occurrence.ItemToClickWith").c_str());
			FlyObject
			i++;
		/*		string conditionsSerializeStr = string(serializeObjectStrActions + ".Conditions").c_str();
				int conditionsAmount = json_object_dotget_number(root_obj, string(conditionsSerializeStr + ".ConditionsAmount").c_str());

				if (conditionsAmount == 0)
					continue;
				
				if(actionNames[i] == "DisplayImage")
					holderAction = parentObject->GetAction(ACTION_DISPLAY_IMAGE);

				else if (actionNames[i] == "ChangeRoom")
					holderAction = parentObject->GetAction(ACTION_CHANGE_ROOM);

				else if (actionNames[i] == "EmitSound")
					holderAction = parentObject->GetAction(ACTION_EMIT_SOUND);

				else if (actionNames[i] == "ModifyVariable")
					holderAction = parentObject->GetAction(ACTION_MOD_VARIABLE);


				int count = 0;
				while (count < conditionsAmount)
				{
					string serializeStr = conditionsSerializeStr + ".Condition_" + to_string(count);

					int conditionType_tmp = json_object_dotget_number(root_obj, string(serializeStr + ".ConditionType").c_str());
					ActionConditionType actionConditionType = (ActionConditionType)conditionType_tmp;

					switch (actionConditionType)
					{
					case CONDITION_IS_VARIABLE:
						holderAction->LoadConditionVariable(root_obj, serializeStr, parentObject->GetParentRoom()->GetBlackboard());
						break;

					case CONDITION_HAS_ITEM:
						holderAction->LoadConditionHasItem(root_obj, serializeStr);
						break;
					}
					count++;
				}*/
			
		}	
	}
}

void SaveAndLoad::LoadDataToRoom(std::string roomDataFilePath, Room* roomToLoad)
{
	// Load Room Blackboard
	Blackboard* roomBB = roomToLoad->CreateBlackboard(); 
	roomBB->LoadData(roomToLoad->GetName() + "_Blackboard");

	// Load Room Data
	JSON_Value* root = json_parse_file(roomDataFilePath.c_str());
	JSON_Object* root_obj = json_value_get_object(root);

	int obj_ammount = json_object_dotget_number(root_obj, "RoomData.ObjectsAmount");

	string thumbnailResourceName = json_object_dotget_string(root_obj, "RoomData.ThumbnailName");
	Texture* thumbnailTexture = (Texture*)ResourceManager::getInstance()->GetResource(thumbnailResourceName.c_str()); 
	roomToLoad->roomThumbnail = thumbnailTexture; 

	int counter = 0;
	while (counter < obj_ammount)
	{
		string serializeObjectStr = "RoomData.Objects" + string(".FlyObject_") + to_string(counter) + string(".");
		instance->CreateFlyObjectFromSavedData(root_obj, serializeObjectStr, roomToLoad);
		counter++;
	}

	// We wait until the object is created to ensure any reference in Conditions is already there 
	counter = 0;
	while (counter < obj_ammount)
	{
		string serializeObjectStr = "RoomData.Objects" + string(".FlyObject_") + to_string(counter) + string(".");
		instance->LoadActionConditions(root_obj, serializeObjectStr, roomToLoad);
		counter++;
	}


	string serialiseStr = "RoomData.UserInterface";

	if(roomToLoad)
		roomToLoad->roomUIHandler->LoadRoomUI(root_obj, serialiseStr);
}

SaveAndLoad::SaveAndLoad()
{

}

void SaveAndLoad::Delete()
{
	delete instance;
}

void SaveAndLoad::SaveAllData()
{
	for (auto& currentRoom : App->moduleWorldManager->roomsInWorldList)
	{
		instance->SaveRoomData(currentRoom);
	}

	// Save Global Blackboard
	App->moduleWorldManager->globalBlackboard->SaveData("Global"); 

	// Save Global Data
	App->moduleWorldManager->SaveConfigData(); 
}
