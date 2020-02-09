#ifndef _GAME_INVENTORY_H_
#define _GAME_INVENTORY_H_

#include <list>
#include "Globals.h"

class FlyObject;
struct GameInventory
{
private:
	static GameInventory* instance;
	GameInventory();

public:
	static GameInventory* getInstance();
	~GameInventory();
	static void CleanUp(); 

	static void AddObjectToInventoryList(FlyObject* newObject);

	static bool IsItemInInventory(UID checkItemUID);

	// Pick and Drop ---
	static FlyObject* PickObjectFromInventory(int index); 
	static void DrawDroppingObject(); 
	static void DropDroppingObjectToRoom(); 

	FlyObject* droppingObject; 

private:
	std::list<FlyObject*> objectsInInventory; 
};

#endif

