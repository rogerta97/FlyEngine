#ifndef _GAME_INVENTORY_H_
#define _GAME_INVENTORY_H_

#include <list>

class FlyObject;
struct GameInventory
{
private:
	static GameInventory* instance;
	GameInventory();

public:
	static GameInventory* getInstance();
	~GameInventory();

	static void AddObjectToInventoryList(FlyObject* newObject);

	// Pick ---
	static FlyObject* PickObjectFromInventory(int index); 

private:
	std::list<FlyObject*> objectsInInventory; 
};

#endif

