#include "SaveAndLoad.h"

#include "MyFileSystem.h"
#include "Application.h"
#include "ModuleRoomManager.h"
#include "Room.h"

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

	int obj_ammount = json_object_dotget_number(root_obj, string(currentRoom->GetName().c_str() + string("ObjectsAmount")).c_str());
}

SaveAndLoad::SaveAndLoad()
{

}

void SaveAndLoad::Delete()
{
	delete instance;
}
