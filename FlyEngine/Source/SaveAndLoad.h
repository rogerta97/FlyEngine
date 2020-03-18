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
	void LoadFollowPathAction(JSON_Object* root_obj, std::string& serializeObjectStrActions, FlyObject* newObject);
	void LoadDisplayAnimationAction(JSON_Object* root_obj, std::string& serializeObjectStrActions, FlyObject* newObject);
	void LoadDisplayTextAction(JSON_Object* root_obj, std::string& serializeObjectStrActions, FlyObject* newObject);
	void LoadEmitSoundAction(JSON_Object* root_obj, std::string& serializeObjectStrActions, FlyObject* newObject);
	void LoadModifyVariableAction(JSON_Object* root_obj, std::string& serializeObjectStrActions, FlyObject* newObject, Room* currentRoom);
	void LoadChangeRoomAction(JSON_Object* root_obj, std::string& serializeObjectStrActions, FlyObject* newObject);
	void LoadDisplayImageAction(JSON_Object* root_obj, std::string& serializeObjectStrActions, FlyObject* newObject);
	static void LoadActionConditions(JSON_Object* root_obj, std::string& serializeObjectStr, Room* currentRoom);

	static void LoadDataToRoom(std::string roomDataFilePath, Room* roomToSave);
};

#endif 


