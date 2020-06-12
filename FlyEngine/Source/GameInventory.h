#ifndef _GAME_INVENTORY_H_
#define _GAME_INVENTORY_H_

#include <list>
#include "Globals.h"
#include "Math/float2.h"
#include "Math/float4.h"

class FlyObject;
class BoundingBox;
class DisplayImageAction;
class AudioClip; 
class InventorySlot
{
public: 
	InventorySlot(float inventoryBgHeigth);
	~InventorySlot();

	FlyObject* GetSlotObject();
	void SetObject(FlyObject* newObject); 

	bool IsEmpty(); 

	BoundingBox* GetSlotBB(); 

public: 
	float2 slotScale;
	float2 viewportScale;
	bool isObjectPicked; 

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
	static void ClearItems(); 

	static void CreateSlots(); 
	static void AddEmptySlot(); 
	static void AddObjectToInventory(FlyObject* newObject);
	static void PlaceObjectInSlot(InventorySlot* currentSlot, FlyObject* newItem);
	static bool IsItemInInventory(UID checkItemUID);
	static void UpdateInventorySlots();
	static bool IsOpened(); 
	static std::list<InventorySlot*>* GetInventorySlots(); 
	static void CheckReturnDroppingObject(); 

	// Draw
	static void OpenInventory();
	static void CloseInventory(bool doSound = true); 
	static void ToggleVisibility(); 

	static void DrawInventory();
	static void DrawPageArrows();
	static void DrawInventoryBackground();
	static void DrawInventorySlots();

	// Pick and Drop ---
	static FlyObject* PickObjectFromInventory(int index); 
	static void DrawDroppingObject(); 
	static FlyObject* DropObjectToRoom();

	// Others ------------
	static BoundingBox* GetBackgroundBB();

	float GetMaxSlots(); 
	void SetMaxSlots(float _maxSlots);

	FlyObject* droppingObject; 

	float4 backgroundColor = float4::zero; 
	float4 slotColor = float4::zero;
	float4 arrowColor = float4::zero;
	bool seePreview = false; 
	bool returnDroppingObject = false; 

private:
	std::list<InventorySlot*> inventorySlots;

	// Draw Vars
	BoundingBox* backgroundBB; 
	BoundingBox* prevPageArrow; 
	BoundingBox* nextPageArrow; 

	DisplayImageAction* slotIconDropingAction;

	AudioClip* openInventorySFX;
	AudioClip* closeInventorySFX;

	int currentPage = 0; 
	float inventoryWidth;
	float inventoryHeigth;
	float slotsInnerPadding; 
	float slotsOutterPadding; 
	float maxSlots; 

	float arrowsWidth;
	float arrowsHeigth;

	bool opened = false; 
};

#endif

