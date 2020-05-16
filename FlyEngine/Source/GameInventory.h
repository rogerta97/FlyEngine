#ifndef _GAME_INVENTORY_H_
#define _GAME_INVENTORY_H_

#include <list>
#include "Globals.h"

class FlyObject;
class BoundingBox;
struct GameInventory
{
private:
	static GameInventory* instance;
	GameInventory();

public:
	static GameInventory* getInstance();
	~GameInventory();
	static void CleanUp(); 

	static void AddObjectToInventory(FlyObject* newObject);
	static bool IsItemInInventory(UID checkItemUID);

	// Draw
	static void OpenInventory();
	static void CloseInventory(); 
	static void ToggleVisibility(); 
	static void DrawInventoryInViewport(); 

	// Pick and Drop ---
	static FlyObject* PickObjectFromInventory(int index); 
	static void DrawDroppingObject(); 
	static void DropDroppingObjectToRoom(); 

	FlyObject* droppingObject; 

private:
	std::list<FlyObject*> objectsInInventory; 

	// Draw Vars
	BoundingBox* backgroundQuad; 
	bool opened = false; 
};

#endif

