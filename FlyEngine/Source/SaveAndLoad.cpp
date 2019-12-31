#include "SaveAndLoad.h"

#include "MyFileSystem.h"
#include "Application.h"
#include "ModuleRoomManager.h"
#include "Room.h"
#include "FlyObject.h"

#include "ModifyVariableAction.h"
#include "DisplayImageAction.h"
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
	// Room Data 
	Room* currentRoom = App->moduleRoomManager->GetSelectedRoom(); 
	instance->SaveRoomData(currentRoom); 
	
	// Blackboard Data
	std::string blackboardFileName = currentRoom->GetName() + "_Blackboard"; 
	currentRoom->GetBlackboard()->SaveData(blackboardFileName.c_str());
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

	// Actions -------
	if (json_object_dothas_value(root_obj, string(serializeObjectStr + string("Actions")).c_str()))
	{
		string serializeObjectStrActions = serializeObjectStr + "Actions.";
		if (json_object_dothas_value(root_obj, string(serializeObjectStrActions + string("DisplayImage")).c_str()))
		{
			int quadWidth = json_object_dotget_number(root_obj, string(serializeObjectStrActions + string("DisplayImage.ImageWidth")).c_str());
			int quadHeight = json_object_dotget_number(root_obj, string(serializeObjectStrActions + string("DisplayImage.ImageHeigth")).c_str());

			string textureName = json_object_dotget_string(root_obj, string(serializeObjectStrActions + string("DisplayImage.TextureName")).c_str());
			string imagePath(MyFileSystem::getInstance()->GetSolutionDirectory() + string("EngineResources\\Images\\" + textureName));

			DisplayImageAction* displayImageAction = newObject->AddDisplayImageAction(imagePath.c_str()); 
		}

		if (json_object_dothas_value(root_obj, string(serializeObjectStrActions + string("ChangeRoom")).c_str()))
		{
			string destinationRoomName = json_object_dotget_string(root_obj, string(serializeObjectStrActions + string("ChangeRoom.Destination")).c_str());
			ChangeRoomAction* changeRoomAction = newObject->AddChangeRoomAction(); 

			Room* room = App->moduleRoomManager->GetRoom(destinationRoomName); 
			changeRoomAction->SetDestination(room); 
		}

		if (json_object_dothas_value(root_obj, string(serializeObjectStrActions + string("ModifyVariable")).c_str()))
		{
			ModifyVariableAction* modifyVariableAction = newObject->AddModifyVariableAction();
			int effectsAmount = json_object_dotget_number(root_obj, string(serializeObjectStrActions + string("ModifyVariable.EffectsAmount")).c_str());

			string baseEffectGroupStr = serializeObjectStrActions + string("ModifyVariable.EffectsGroup.");
			for (int i = 0; i < effectsAmount; i++)
			{
				ModifyVariableEffect* newVariableEffect = new ModifyVariableEffect(); 

				string baseEffectStr = baseEffectGroupStr + "Effect_" + to_string(i) + "."; 
				string targetVariableName = json_object_dotget_string(root_obj, string(baseEffectStr + "TargetVariableName").c_str());

				newVariableEffect->targetVariable = currentRoom->GetBlackboard()->GetVariable(targetVariableName); 
				int v_operator = json_object_dotget_number(root_obj, string(baseEffectStr + "OperatorType").c_str());
				newVariableEffect->variableOperatorType = (VariableOperatorType)v_operator; 
				newVariableEffect->incIntegerValue = json_object_dotget_number(root_obj, string(baseEffectStr + "IncIntegerValue").c_str());
				newVariableEffect->nextToggleValue = json_object_dotget_boolean(root_obj, string(baseEffectStr + "NextToggleValue").c_str());

				modifyVariableAction->AddEffect(newVariableEffect); 
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
 
	newObject->FitObjectUtils();
}

void SaveAndLoad::LoadDataToRoom(std::string roomDataFilePath, Room* roomToLoad)
{
	// Load Room Blackboard 
	std::string blackboardFileName = roomToLoad->GetName() + "_Blackboard";
	roomToLoad->GetBlackboard()->LoadData(blackboardFileName);

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
}

SaveAndLoad::SaveAndLoad()
{

}

void SaveAndLoad::Delete()
{
	delete instance;
}
