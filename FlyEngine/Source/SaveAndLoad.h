#ifndef _SAVE_AND_LOAD_H_
#define _SAVE_AND_LOAD_H_

#include "JSON/parson.h"
#include <string>

class Room; 
struct SaveAndLoad {

private:
	static SaveAndLoad* instance;
	SaveAndLoad();

public:
	static SaveAndLoad* getInstance();
	~SaveAndLoad();

	void SaveCurrentRoomData();
	void LoadDataToCurrentRoom(std::string roomDataFilePath);

	void CreateFlyObjectFromSavedData(JSON_Object* root_obj, std::string& serializeObjectStr, Room* currentRoom);

	static void Delete();
};

#endif 


