#ifndef _SAVE_AND_LOAD_H_
#define _SAVE_AND_LOAD_H_

#include "JSON/parson.h"
#include "Globals.h"
#include <string>

class Room; 
class FlyObject;
struct SaveAndLoad {

private:
	static SaveAndLoad* instance;
	SaveAndLoad();

public:
	static SaveAndLoad* getInstance();
	~SaveAndLoad();
	static void Delete();

	static void SaveAllData(); 

	// Save ----
	static void SaveCurrentRoomData();

	static void SaveRoomData(std::string roomName);
	static void SaveRoomData(UID hello);
	static void SaveRoomData(Room* roomToSave);

	static void SaveSelectedRoomToOnPlayData(); 
	static void LoadOnPlayData(); 

	static void SaveInitFile();

	// Load ----
	static void LoadDataToSelectedRoom(std::string roomDataFilePath);
	static void CreateFlyObjectFromSavedData(JSON_Object* root_obj, std::string& serializeObjectStr, Room* currentRoom);

	static void LoadFollowPathAction(JSON_Object* root_obj, std::string& serializeObjectStrActions, FlyObject* newObject);
	static void LoadDisplayAnimationAction(JSON_Object* root_obj, std::string& serializeObjectStrActions, FlyObject* newObject);
	static void LoadDisplayTextAction(JSON_Object* root_obj, std::string& serializeObjectStrActions, FlyObject* newObject);
	static void LoadEmitSoundAction(JSON_Object* root_obj, std::string& serializeObjectStrActions, FlyObject* newObject);
	static void LoadModifyVariableAction(JSON_Object* root_obj, std::string& serializeObjectStrActions, FlyObject* newObject, Room* currentRoom);
	static void LoadChangeRoomAction(JSON_Object* root_obj, std::string& serializeObjectStrActions, FlyObject* newObject);
	static void LoadDisplayImageAction(JSON_Object* root_obj, std::string& serializeObjectStrActions, FlyObject* newObject);
	static void LoadActionConditions(JSON_Object* root_obj, std::string& serializeObjectStr, Room* currentRoom);
	static void LoadDialogueAction(JSON_Object* root_obj, std::string& serializeObjectStrActions, FlyObject* newObject);

	static void LoadDataToRoom(std::string roomDataFilePath, Room* roomToSave);
};

#endif 


