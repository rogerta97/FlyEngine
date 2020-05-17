#ifndef _GAME_INVENTORY_H_
#define _GAME_INVENTORY_H_

#include <list>
#include "Globals.h"


class FlyObject;
class BoundingBox;
class InventorySlot
{
public: 
	InventorySlot();
	~InventorySlot();

	FlyObject* GetSlotObject();
	void SetObject(FlyObject* newObject); 

private:
	FlyObject* slotObject; 
	BoundingBox* slotBG; 
};

struct GameInventory
{
private:
	static GameInventory* instance;
	GameInventory();

public:
	static GameInventory* getInstance();
	~GameInventory();
	static void CleanUp(); 

	static void AddEmptySlot(); 
	static void AddObjectToInventory(FlyObject* newObject);
	static bool IsItemInInventory(UID checkItemUID);

	// Draw
	static void OpenInventory();
	static void CloseInventory(); 
	static void ToggleVisibility(); 

	static void DrawInventoryInViewport(); 
	static void DrawInventorySlots(); 

	// Pick and Drop ---
	static FlyObject* PickObjectFromInventory(int index); 
	static void DrawDroppingObject(); 
	static void DropDroppingObjectToRoom(); 

	FlyObject* droppingObject; 

private:
	std::list<InventorySlot*> inventorySlots;

	// Draw Vars
	BoundingBox* backgroundQuad; 

	int showingPage = 0; 
	float inventoryWidth;
	float inventoryHeigth;
	bool opened = false; 
};

#endif

