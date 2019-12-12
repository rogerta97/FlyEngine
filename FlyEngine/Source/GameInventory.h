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

	std::list<FlyObject*> objectsInInventory; 

};

#endif

