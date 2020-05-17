#include "GameInventory.h"

#include "Application.h"
#include "ModuleImGui.h"
#include "GameViewportDockPanel.h"
#include "ModuleWorldManager.h"
#include "FlyObject.h"
#include "ModuleManager.h"
#include "BoundingBox.h"
#include "Room.h"
#include "Quad.h"
#include "MathGeoLib.h"
#include "mmgr.h"

GameInventory* GameInventory::instance = 0; 

GameInventory::GameInventory()
{
	droppingObject = nullptr; 

	inventoryWidth = 900;
	inventoryHeigth = 160;
	showingPage = 0; 

	backgroundQuad = new BoundingBox();
	backgroundQuad->SetSize(inventoryWidth, inventoryHeigth);
	backgroundQuad->SetPositionInc(float2(0, -423)); 

	opened = false; 

	AddEmptySlot();
	AddEmptySlot();
	AddEmptySlot();
	AddEmptySlot();
}

GameInventory* GameInventory::getInstance()
{
	if (instance == 0)
		instance = new GameInventory();

	return instance;
}

GameInventory::~GameInventory()
{

}

void GameInventory::CleanUp()
{
	delete instance; 
}

void GameInventory::AddEmptySlot()
{
	InventorySlot* newSlot = new InventorySlot();
	instance->inventorySlots.push_back(newSlot);
}

void GameInventory::AddObjectToInventory(FlyObject* newObject)
{
	if (newObject == nullptr)
		return; 

	InventorySlot* newInvSlot = new InventorySlot(); 
	newInvSlot->SetObject(newObject); 

	instance->inventorySlots.push_back(newInvSlot);
}

bool GameInventory::IsItemInInventory(UID checkItemUID)
{
	for (auto& currentSlot : instance->inventorySlots)
	{
		if (currentSlot->GetSlotObject()->GetUID() == checkItemUID)
			return true; 
	}

	return false; 
}

void GameInventory::OpenInventory()
{
	instance->opened = true; 
}

void GameInventory::CloseInventory()
{
	instance->opened = false;
}

void GameInventory::ToggleVisibility()
{
	instance->opened = !instance->opened;
}

void GameInventory::DrawInventoryInViewport()
{
	if (!instance->opened)
		return; 

	if (instance->backgroundQuad != nullptr)
	{
		instance->backgroundQuad->Draw(true, float4(1, 1, 1, 1));
		//instance->backgroundQuad->DrawSquare(); 
	}		
}

void GameInventory::DrawInventorySlots()
{
}

FlyObject* GameInventory::PickObjectFromInventory(int index)
{
	int count = 0; 
	FlyObject* retObject = nullptr; 
	for (auto currentSlot = instance->inventorySlots.begin(); currentSlot != instance->inventorySlots.end(); currentSlot++)
	{
		if (count++ == index)
		{	
			retObject = (*currentSlot)->GetSlotObject(); 
			break;
		}
	}

	instance->droppingObject = retObject; 
	return retObject;
}

void GameInventory::DrawDroppingObject()
{
	if (instance->droppingObject == nullptr)
		return; 

	// Set Object Position to Mouse Pos 
	float2 mouseRelativePos = App->moduleImGui->gameViewportDockPanel->GetMouseRelativePosition(); 
	mouseRelativePos = App->moduleImGui->gameViewportDockPanel->GetMouseGamePos();
	instance->droppingObject->transform->SetPosition(mouseRelativePos); 
	instance->droppingObject->FitObjectUtils();

	instance->droppingObject->DrawVisualLayer();
	instance->droppingObject->DrawOverlaysLayer();

	flog("Object Dropping"); 
}

void GameInventory::DropDroppingObjectToRoom()
{
	for (auto it = instance->inventorySlots.begin(); it != instance->inventorySlots.end(); it++)
	{
		// TODO: this should be done by UID for seccurity
		if (instance->droppingObject->GetName() == (*it)->GetSlotObject()->GetName())
		{
			Room* selectedRoom = App->moduleWorldManager->GetSelectedRoom(); 
			selectedRoom->AddFlyObject((*it)->GetSlotObject());
			instance->inventorySlots.erase(it);
			instance->droppingObject = nullptr; 
			break; 
		}
	}
}

InventorySlot::InventorySlot()
{
}

InventorySlot::~InventorySlot()
{
}

FlyObject* InventorySlot::GetSlotObject()
{
	return slotObject; 
}

void InventorySlot::SetObject(FlyObject* newObject)
{
	if(newObject != nullptr)
		slotObject = newObject;
}
