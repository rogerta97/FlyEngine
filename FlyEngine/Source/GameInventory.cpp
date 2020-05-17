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

#define SLOTS_PER_PAGE 5

GameInventory* GameInventory::instance = 0; 

GameInventory::GameInventory()
{
	droppingObject = nullptr; 

	inventoryWidth = 900;
	inventoryHeigth = 160;
	currentPage = 0; 
	slotsInnerPadding = 30;
	slotsOutterPadding = 20; 
	arrowsWidth = 60; 
	arrowsHeigth = 90; 

	backgroundQuad = new BoundingBox();
	backgroundQuad->SetSize(inventoryWidth, inventoryHeigth);
	backgroundQuad->SetPositionInc(float2(0, -423)); 

	nextPageArrow = new BoundingBox();
	nextPageArrow->SetSize(arrowsWidth, arrowsHeigth);
	nextPageArrow->SetPositionInc(float2(backgroundQuad->GetTopLeft().x, -423));

	prevPageArrow = new BoundingBox();
	prevPageArrow->SetSize(arrowsWidth, arrowsHeigth);
	prevPageArrow->SetPositionInc(float2(backgroundQuad->GetMaxPoint().x, -423));

	opened = false; 
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

void GameInventory::CreateSlots(int amount)
{
	int counter = 0;
	while (counter < amount)
	{
		instance->AddEmptySlot();
		counter++; 
	}
}

void GameInventory::AddEmptySlot()
{
	InventorySlot* newSlot = new InventorySlot(instance->inventoryHeigth);
	instance->inventorySlots.push_back(newSlot);
}

void GameInventory::AddObjectToInventory(FlyObject* newObject)
{
	//if (newObject == nullptr)
	//	return; 

	//InventorySlot* newInvSlot = new InventorySlot(); 
	//newInvSlot->SetObject(newObject); 

	//instance->inventorySlots.push_back(newInvSlot);
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

void GameInventory::DrawInventory()
{
	if (!instance->opened)
		return; 

	instance->DrawInventoryBackground();
	instance->DrawInventorySlots();
	instance->DrawPageArrows();
}

void GameInventory::DrawPageArrows()
{
	instance->prevPageArrow->Draw(true, float4(1, 0, 0, 1));
	instance->nextPageArrow->Draw(true, float4(1, 0, 0, 1));
}

void GameInventory::DrawInventoryBackground()
{
	if (instance->backgroundQuad != nullptr)
	{
		instance->backgroundQuad->Draw(true, float4(1, 1, 1, 1));
	}
}

void GameInventory::DrawInventorySlots()
{
	float2 inventoryBackgroundTopLeft = float2(instance->backgroundQuad->GetMinPoint().x, instance->backgroundQuad->GetMaxPoint().y);

	int startDrawingIndex = instance->currentPage * SLOTS_PER_PAGE;

	int counter = 0; 

	float2 pen = inventoryBackgroundTopLeft + float2(40,11);

	for (std::list<InventorySlot*>::iterator it = instance->inventorySlots.begin(); it != instance->inventorySlots.end(); it++)
	{
		if (counter >= startDrawingIndex && counter < startDrawingIndex + SLOTS_PER_PAGE)
		{
			(*it)->GetSlotBB()->SetPosition(pen, true);
			(*it)->GetSlotBB()->Draw(true, float4(0,0,0,1)); 
			pen.x += (*it)->GetSlotBB()->GetSize().x + instance->slotsInnerPadding; 
		}

		counter++;
	}
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

InventorySlot::InventorySlot(float inventoryBgHeigth)
{
	slotObject = nullptr;

	slotBB = new BoundingBox();
	slotBB->SetSize(inventoryBgHeigth - 20.0f, inventoryBgHeigth - 20.0f);
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

BoundingBox* InventorySlot::GetSlotBB()
{
	return slotBB;
}
