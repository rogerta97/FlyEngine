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
#include "ChangeRoomAction.h"

#include <fstream>

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

void SaveAndLoad::LoadOnPlayToSelectedRoom()
{
	string provisionalPath = MyFileSystem::getInstance()->GetSavedDataDirectory() + "OnPlaySaveData.json";
	instance->LoadDataToCurrentRoom(provisionalPath);
}

void SaveAndLoad::LoadDataToCurrentRoom(std::string roomDataFilePath)
{
	// Get Current Room 
	Room* currentRoom = App->moduleRoomManager->GetSelectedRoom();
	LoadDataToRoom(roomDataFilePath, currentRoom); 
}

void SaveAndLoad::CreateFlyObjectFromSavedData(JSON_Object* root_obj, std::string& serializeObjectStr, Room* currentRoom)
{
	string newObjectName = json_object_dotget_string(root_obj, string(serializeObjectStr + string("Name")).c_str());
	FlyObject* newObject = currentRoom->CreateFlyObject(newObjectName.c_str());
	newObject->SetUID(json_object_dotget_number(root_obj, string(serializeObjectStr + string("UID")).c_str())); 

	if (json_object_dothas_value(root_obj, string(serializeObjectStr + string("Description")).c_str())) 
	{
		newObject->SetDescription(json_object_dotget_string(root_obj, string(serializeObjectStr + string("Description")).c_str()));
	}

	// Transform ------
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
			string texturePath = json_object_dotget_string(root_obj, string(serializeObjectStrActions + string("DisplayImage.TextureName")).c_str());
			DisplayImageAction* displayImageAction = newObject->AddDisplayImageAction(texturePath.c_str());
			displayImageAction->LoadOccurrence(root_obj, serializeObjectStrActions + string("DisplayImage.Occurrence.")); 
		}

		if (json_object_dothas_value(root_obj, string(serializeObjectStrActions + string("ChangeRoom")).c_str()))
		{
			string destinationRoomName = json_object_dotget_string(root_obj, string(serializeObjectStrActions + string("ChangeRoom.Destination")).c_str());
			ChangeRoomAction* changeRoomAction = newObject->AddChangeRoomAction(); 
			changeRoomAction->LoadOccurrence(root_obj, serializeObjectStrActions + string("ChangeRoom.Occurrence."));

			Room* room = App->moduleRoomManager->GetRoom(destinationRoomName); 
			changeRoomAction->SetDestination(room); 
		}

		if (json_object_dothas_value(root_obj, string(serializeObjectStrActions + string("ModifyVariable")).c_str()))
		{
			int effectsAmount = json_object_dotget_number(root_obj, string(serializeObjectStrActions + string("ModifyVariable.EffectsAmount")).c_str());
			ModifyVariableAction* modifyVariableAction = newObject->AddModifyVariableAction();
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

		if (json_object_dothas_value(root_obj, string(serializeObjectStrActions + string("EmitSound")).c_str()))
		{
			string audioClipPath = json_object_dotget_string(root_obj, string(serializeObjectStrActions + string("EmitSound.Path")).c_str());
			EmitSoundAction* emitSoundAction = newObject->AddEmitSoundAction();
			emitSoundAction->LoadOccurrence(root_obj, serializeObjectStrActions + string("EmitSound.Occurrence."));

			if (audioClipPath != "None")
			{
				string audioClipResourceName = MyFileSystem::getInstance()->GetLastPathItem(audioClipPath, false);
				AudioClip* soundClipResource = (AudioClip*)ResourceManager::getInstance()->GetResource(audioClipResourceName.c_str());
				emitSoundAction->audioClip = soundClipResource; 
			}
		}
	}

	// Clickable Area
	newObject->clickableAreaActive = json_object_dotget_boolean(root_obj, string(serializeObjectStr + string("ClickableArea.Active")).c_str());
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

void SaveAndLoad::LoadDataToRoom(std::string roomDataFilePath, Room* roomToLoad)
{
	// Load Room Blackboard
	Blackboard* roomBB = roomToLoad->CreateBlackboard(); 
	roomBB->LoadData(roomToLoad->GetName() + "_Blackboard");

	// Load Room Data
	JSON_Value* root = json_parse_file(roomDataFilePath.c_str());
	JSON_Object* root_obj = json_value_get_object(root);

	int obj_ammount = json_object_dotget_number(root_obj, string(roomToLoad->GetName().c_str() + string(".ObjectsAmount")).c_str());

	int counter = 0;
	while (counter < obj_ammount)
	{
		string serializeObjectStr = roomToLoad->GetName().c_str() + string(".FlyObject_") + to_string(counter) + string(".");
		instance->CreateFlyObjectFromSavedData(root_obj, serializeObjectStr, roomToLoad);
		counter++;
	}

	string serialiseStr = roomToLoad->GetName() + ".UserInterface";

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
