#ifndef _GAME_INVENTORY_H_
#define _GAME_INVENTORY_H_

#include <list>
#include "Globals.h"


class FlyObject;
class BoundingBox;
class InventorySlot
{
public: 
	InventorySlot(float inventoryBgHeigth);
	~InventorySlot();

	FlyObject* GetSlotObject();
	void SetObject(FlyObject* newObject); 

	BoundingBox* GetSlotBB(); 

private:
	FlyObject* slotObject; 
	BoundingBox* slotBB; 
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

	static void CreateSlots(int amount); 
	static void AddEmptySlot(); 
	static void AddObjectToInventory(FlyObject* newObject);
	static bool IsItemInInventory(UID checkItemUID);

	// Draw
	static void OpenInventory();
	static void CloseInventory(); 
	static void ToggleVisibility(); 

	static void DrawInventory();
	static void DrawPageArrows();
	static void DrawInventoryBackground();

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
	BoundingBox* nextPageArrow; 
	BoundingBox* prevPageArrow; 

	int currentPage = 0; 
	float inventoryWidth;
	float inventoryHeigth;
	float slotsInnerPadding; 
	float slotsOutterPadding; 

	float arrowsWidth;
	float arrowsHeigth;

	bool opened = false; 
};

#endif

