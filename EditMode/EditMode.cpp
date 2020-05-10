// EditMode.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <filesystem>
#include <JSON/parson.h>

int main()
{
	char full[_MAX_PATH];
	_fullpath(full, ".\\", _MAX_PATH);

	// Load Path Data 
	std::string initFilePath = full + std::string("Resources\\EngineResources\\EngineSavedData\\Init.json");
	JSON_Value* root = json_parse_file(initFilePath.c_str());
	JSON_Object* root_obj = json_value_get_object(root);

	int firstRoomUID = json_object_dotget_number(root_obj, "InitData.StartRoomUID");
	std::string currentRoomName = json_object_dotget_string(root_obj, "InitData.ProjectName");
	int gameMode = json_object_dotget_number(root_obj, "InitData.GameMode");

	// Save New File Modified 
	JSON_Value* scene_v = json_value_init_object();
	JSON_Object* scene_obj = json_value_get_object(scene_v);

	json_object_dotset_number(scene_obj, "InitData.StartRoomUID", 0);
	json_object_dotset_string(scene_obj, "InitData.ProjectName", "");
	json_object_dotset_boolean(scene_obj, "InitData.GameMode", 0);

	json_serialize_to_file(scene_v, initFilePath.c_str());

    std::cout << "Fly Engine is now in Edit Mode!";
    getchar();
	return 0;
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
