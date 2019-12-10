#include "SaveAndLoad.h"

#include "MyFileSystem.h"
#include "Application.h"
#include "ModuleRoomManager.h"
#include "Room.h"
#include "FlyObject.h"

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
	instance->Delete();
}

void SaveAndLoad::SaveCurrentRoomData()
{
	// Get Current Room 
	Room* currentRoom = App->moduleRoomManager->GetSelectedRoom(); 

	// Open File To Save 
	std::ofstream stream;
	string testPath = MyFileSystem::getInstance()->GetGameDirectory() + "\\Resources\\EngineSavedData\\TestFile.json";
	stream.open(testPath, std::fstream::out);

	// Save Data 
	JSON_Value* scene_v = json_value_init_object();
	JSON_Object* scene_obj = json_value_get_object(scene_v);

	//Save Meta Info
	currentRoom->SaveRoomData(scene_obj); 

	json_serialize_to_file(scene_v, testPath.c_str());
}

void SaveAndLoad::LoadDataToCurrentRoom(std::string roomDataFilePath)
{
	// Get Current Room 
	Room* currentRoom = App->moduleRoomManager->GetSelectedRoom();

	// Open File To Read
	JSON_Value* root = json_parse_file(roomDataFilePath.c_str());
	JSON_Object* root_obj = json_value_get_object(root);

	int obj_ammount = json_object_dotget_number(root_obj, string(currentRoom->GetName().c_str() + string(".ObjectsAmount")).c_str());

	int counter = 0; 
	while (counter < obj_ammount)
	{
		string serializeObjectStr = currentRoom->GetName().c_str() + string(".FlyObject_") + to_string(counter) + string("."); 
		CreateFlyObjectFromSavedData(root_obj, serializeObjectStr, currentRoom);
		counter++;
	}
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
	}

	// Clickable Area
	newObject->clickableAreaActive = json_object_dotget_boolean(root_obj, string(serializeObjectStr + string("ClickableArea.Active")).c_str());
 
	newObject->FitObjectUtils();
}

SaveAndLoad::SaveAndLoad()
{

}

void SaveAndLoad::Delete()
{
	delete instance;
}
