// EditMode.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <JSON/parson.h>

int main()
{
    std::cout << "Hello World!\n";

	//std::string hello = SDL_GetBasePath();

	JSON_Value* root = json_parse_file("ldskhfldsaf");
	JSON_Object* root_obj = json_value_get_object(root);

	//string currentRoomName = json_object_dotget_string(root_obj, "RoomData.Name");
	//Room* newRoom = App->moduleRoomManager->CreateEmptyRoom(currentRoomName);

    getchar();
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
