#ifndef _SAVE_AND_LOAD_H_
#define _SAVE_AND_LOAD_H_

#include "JSON/parson.h"
#include "Globals.h"
#include <string>

class Room; 
struct SaveAndLoad {

private:
	static SaveAndLoad* instance;
	SaveAndLoad();

public:
	static SaveAndLoad* getInstance();
	~SaveAndLoad();
	static void Delete();

	// Save ----
	static void SaveCurrentRoomData();

	static void SaveRoomData(std::string roomName);
	static void SaveRoomData(UID hello);
	static void SaveRoomData(Room* roomToSave);

	static void SaveSelectedRoomToOnPlayData(); 
	static void LoadOnPlayData(); 

	// Load ----
	static void LoadDataToSelectedRoom(std::string roomDataFilePath);
	static void CreateFlyObjectFromSavedData(JSON_Object* root_obj, std::string& serializeObjectStr, Room* currentRoom);

	static void LoadDataToRoom(std::string roomDataFilePath, Room* roomToSave);
};

#endif 


