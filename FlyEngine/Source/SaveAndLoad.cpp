#include "SaveAndLoad.h"

#include "MyFileSystem.h"
#include "Application.h"
#include "ModuleRoomManager.h"
#include "ModifyVariableAction.h"
#include "ResourceManager.h"
#include "GameViewportDockPanel.h"
#include "ModuleImGui.h"
#include "RoomUIHandler.h"
#include "Room.h"
#include "FlyObject.h"

#include "EmitSoundAction.h"
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
	Room* currentRoom = App->moduleRoomManager->GetSelectedRoom(); 
	instance->SaveRoomData(currentRoom); 
}

void SaveAndLoad::SaveRoomData(std::string roomName)
{
	instance->SaveRoomData(App->moduleRoomManager->GetRoom(roomName));
}

void SaveAndLoad::SaveRoomData(UID roomUID)
{
	instance->SaveRoomData(App->moduleRoomManager->GetRoom(roomUID));
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
}

void SaveAndLoad::SaveSelectedRoomToOnPlayData()
{
	Room* selectedRoom = App->moduleRoomManager->GetSelectedRoom(); 

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
	Room* nextSelectedRoom = App->moduleRoomManager->GetRoom(selectedWhenPlayRoomUID); 
	App->moduleRoomManager->SetSelectedRoom(nextSelectedRoom); 

	App->moduleRoomManager->GetSelectedRoom()->CleanUp(); 

	instance->LoadDataToSelectedRoom(provisionalPath);
}

void SaveAndLoad::LoadDataToSelectedRoom(std::string roomDataFilePath)
{
	// Get Current Room 
	Room* currentRoom = App->moduleRoomManager->GetSelectedRoom();
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
	else	
		newObject = currentRoom->CreateFlyObject(newObjectName.c_str());
	
	// UID --
	newObject->SetUID(json_object_dotget_number(root_obj, string(serializeObjectStr + string("UID")).c_str())); 
	//newObject->SetUID(RandomNumberGenerator::getInstance()->GenerateUID()); 

	if (json_object_dothas_value(root_obj, string(serializeObjectStr + string("Description")).c_str())) 
	{
		newObject->SetDescription(json_object_dotget_string(root_obj, string(serializeObjectStr + string("Description")).c_str()));
	}

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

	// Create Actions -------
	if (json_object_dothas_value(root_obj, string(serializeObjectStr + string("Actions")).c_str()))
	{
		string serializeObjectStrActions = serializeObjectStr + "Actions.";
		if (json_object_dothas_value(root_obj, string(serializeObjectStrActions + string("DisplayImage")).c_str()))
		{
			string textureName = json_object_dotget_string(root_obj, string(serializeObjectStrActions + string("DisplayImage.TextureName")).c_str());
			DisplayImageAction* displayImageAction = newObject->AddDisplayImageAction(textureName.c_str());

			displayImageAction->LoadOccurrence(root_obj, serializeObjectStrActions + string("DisplayImage.Occurrence.")); 
			//displayImageAction->LoadConditions(root_obj, serializeObjectStrActions + string("DisplayImage.Conditions.")); 
		}

		if (json_object_dothas_value(root_obj, string(serializeObjectStrActions + string("ChangeRoom")).c_str()))
		{
			UID destinationRoomUID = json_object_dotget_number(root_obj, string(serializeObjectStrActions + string("ChangeRoom.Destination")).c_str());
			ChangeRoomAction* changeRoomAction = newObject->AddChangeRoomAction(); 

			changeRoomAction->LoadOccurrence(root_obj, serializeObjectStrActions + string("ChangeRoom.Occurrence."));
			//changeRoomAction->LoadConditions(root_obj, serializeObjectStrActions + string("ChangeRoom.Conditions."));

			Room* room = App->moduleRoomManager->GetRoom(destinationRoomUID);
			changeRoomAction->SetDestination(room); 
		}

		if (json_object_dothas_value(root_obj, string(serializeObjectStrActions + string("ModifyVariable")).c_str()))
		{
			int effectsAmount = json_object_dotget_number(root_obj, string(serializeObjectStrActions + string("ModifyVariable.EffectsAmount")).c_str());
			ModifyVariableAction* modifyVariableAction = newObject->AddModifyVariableAction();

			modifyVariableAction->LoadOccurrence(root_obj, serializeObjectStrActions + string("ModifyVariable.Occurrence."));
			//modifyVariableAction->LoadConditions(root_obj, serializeObjectStrActions + string("ModifyVariable.Conditions."));

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

		if (json_object_dothas_value(root_obj, string(serializeObjectStrActions + string("EmitSound")).c_str()))
		{
			string audioClipPath = json_object_dotget_string(root_obj, string(serializeObjectStrActions + string("EmitSound.Path")).c_str());
			EmitSoundAction* emitSoundAction = newObject->AddEmitSoundAction();

			emitSoundAction->LoadOccurrence(root_obj, serializeObjectStrActions + string("EmitSound.Occurrence."));
			//emitSoundAction->LoadConditions(root_obj, serializeObjectStrActions + string("EmitSound.Conditions."));

			if (audioClipPath != "None")
			{
				string audioClipResourceName = MyFileSystem::getInstance()->GetLastPathItem(audioClipPath, false);
				AudioClip* soundClipResource = (AudioClip*)ResourceManager::getInstance()->GetResource(audioClipResourceName.c_str());
				emitSoundAction->audioClip = soundClipResource; 
			}
		}


		if (json_object_dothas_value(root_obj, string(serializeObjectStrActions + string("DisplayText")).c_str()))
		{
			DisplayTextAction* displayTextAction = newObject->AddDisplayTextAction();
			displayTextAction->LoadOccurrence(root_obj, serializeObjectStrActions + string("DisplayText.Occurrence."));

			string serializeDisplayTextStr = serializeObjectStrActions + "DisplayText."; 

			string fontTmp = json_object_dotget_string(root_obj, string(serializeObjectStrActions + string("TextFont")).c_str());
			if (fontTmp != "None")
			{
				Font* textFont = (Font*)ResourceManager::GetResource(fontTmp.c_str(), RESOURCE_FONT);

				if(textFont)
					displayTextAction->SetFont(textFont);
			}

			string textTmp = json_object_dotget_string(root_obj, string(serializeObjectStrActions + string("Text")).c_str());
			if (textTmp != "None")
			{
				displayTextAction->SetText(textTmp); 
			}
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
		const char* actionNames[] = 
		{ 
			"DisplayImage", 
			"ChangeRoom", 
			"EmitSound", 
			"ModifyVariable" 
		}; 

		for (int i = 0; i < 4; i++)
		{
			string serializeObjectStrActions = serializeObjectStr + "Actions." + actionNames[i];
			if (json_object_dothas_value(root_obj, string(serializeObjectStrActions).c_str()))
			{
				Action* holderAction = nullptr;
				string conditionsSerializeStr = string(serializeObjectStrActions + ".Conditions").c_str();
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
				}
			}
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
