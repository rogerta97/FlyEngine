#include "GameInventory.h"
#include "ResourceManager.h"
#include "Application.h"
#include "ModuleImGui.h"
#include "GameViewportDockPanel.h"
#include "ModuleWorldManager.h"
#include "FlyObject.h"
#include "ModuleManager.h"
#include "Gizmos.h"
#include "DisplayImageAction.h"
#include "DisplayImageAction.h"
#include "BoundingBox.h"
#include "Room.h"
#include "Quad.h"
#include "Texture.h"
#include "MathGeoLib.h"

#include "imgui.h"

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
	arrowsWidth = 25; 
	arrowsHeigth = 105; 
	maxSlots = 5;

	slotIconDropingAction = new DisplayImageAction(nullptr); 
	Texture* tex = (Texture*)ResourceManager::getInstance()->GetResource("DropItemSlot");
	slotIconDropingAction->CreateImage(tex->GetPath());

	backgroundColor = float4(1, 1, 1, 1);
	slotColor = float4(0, 0, 0, 1);
	arrowColor = float4(1, 0, 0, 1);

	backgroundBB = new BoundingBox();
	backgroundBB->SetSize(inventoryWidth, inventoryHeigth);
	backgroundBB->SetPositionInc(float2(0, -423)); 

	prevPageArrow = new BoundingBox();
	prevPageArrow->SetSize(arrowsWidth, arrowsHeigth);
	prevPageArrow->SetPositionInc(float2(backgroundBB->GetTopLeft().x, -423));

	nextPageArrow = new BoundingBox();
	nextPageArrow->SetSize(arrowsWidth, arrowsHeigth);
	nextPageArrow->SetPositionInc(float2(backgroundBB->GetMaxPoint().x, -423));

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

void GameInventory::ClearItems()
{
	for (auto& currentSlot : instance->inventorySlots)
	{
		currentSlot->SetObject(nullptr); 
	}
}

void GameInventory::CreateSlots()
{
	int counter = 0;
	while (counter < 20)
	{
		instance->AddEmptySlot();
		counter++; 
	}

	instance->UpdateInventorySlots();
}

void GameInventory::AddEmptySlot()
{
	InventorySlot* newSlot = new InventorySlot(instance->inventoryHeigth);
	instance->inventorySlots.push_back(newSlot);
}

void GameInventory::AddObjectToInventory(FlyObject* newItem)
{
	if (newItem->isPicked)
		return; 

	for (auto& currentSlot : instance->inventorySlots)
	{
		if (currentSlot->IsEmpty())
		{	
			currentSlot->viewportScale = newItem->transform->GetScale(); 

			// Adjust object position
			float ar = App->moduleImGui->gameViewportDockPanel->GetAspectRatio();
			float2 center = currentSlot->GetSlotBB()->GetCenter();
			newItem->transform->SetPosition(center / ar); 

			// Adjust object scale 
			bool isObjectVertical = newItem->GetDisplayImageAction()->IsVertical();

			if (isObjectVertical)
			{
				DisplayImageAction* newObjectDisplayImage = newItem->GetDisplayImageAction();
				float itemHeigth = newItem->gizmos->selectGizmo->objectBorderBox->GetSize().y;
				float slotHeigth = currentSlot->GetSlotBB()->GetSize().y; 
				float increment = slotHeigth / itemHeigth;

				float2 currentItemScale = newItem->transform->GetScale(); 
				float2 desiredItemScale = currentItemScale * increment; 

				newItem->transform->SetScale(desiredItemScale); 
			}
			else
			{
				DisplayImageAction* newObjectDisplayImage = newItem->GetDisplayImageAction();
				float itemWidth = newItem->gizmos->selectGizmo->objectBorderBox->GetSize().x; 
				float slotHWidth = currentSlot->GetSlotBB()->GetSize().x;
				float increment = slotHWidth / itemWidth;

				float2 currentItemScale = newItem->transform->GetScale();
				float2 desiredItemScale = currentItemScale * increment;

				newItem->transform->SetScale(desiredItemScale);
			}

			currentSlot->slotScale = newItem->transform->GetScale();
			currentSlot->SetObject(newItem);

			Room* selectedRoom = App->moduleWorldManager->GetSelectedRoom();
			if (selectedRoom != nullptr)
			{
				App->moduleManager->AddDeleteFromListObject(newItem);
			}

			return;
		}
	}
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
	if(App->isEngineInPlayMode)
		instance->opened = !instance->opened;
}

void GameInventory::DrawInventory()
{
	if ((App->isEngineInPlayMode && instance->opened) || (!App->isEngineInPlayMode && instance->seePreview))
	{
		instance->DrawInventoryBackground();
		instance->DrawInventorySlots();
		instance->DrawPageArrows();
	}
}

void GameInventory::DrawPageArrows()
{
	instance->nextPageArrow->Draw(true, instance->arrowColor);
	instance->prevPageArrow->Draw(true, instance->arrowColor);

	if (instance->nextPageArrow->IsBoxClicked())
	{
		if(instance->currentPage < 3)
			instance->currentPage++;
	}


	if (instance->prevPageArrow->IsBoxClicked())
	{
		if (instance->currentPage > 0)
			instance->currentPage--;
	}
}

void GameInventory::DrawInventoryBackground()
{
	if (instance->backgroundBB != nullptr)
	{
		instance->backgroundBB->Draw(true, instance->backgroundColor);
	}
}

void GameInventory::UpdateInventorySlots()
{
	float2 inventoryBackgroundTopLeft = float2(instance->backgroundBB->GetMinPoint().x, instance->backgroundBB->GetMaxPoint().y);

	int startDrawingIndex = instance->currentPage * SLOTS_PER_PAGE;

	int counter = 0;

	float2 pen = inventoryBackgroundTopLeft + float2(40, 11);

	for (std::list<InventorySlot*>::iterator it = instance->inventorySlots.begin(); it != instance->inventorySlots.end(); it++)
	{
		if (counter >= startDrawingIndex && counter < startDrawingIndex + SLOTS_PER_PAGE)
		{
			(*it)->GetSlotBB()->SetPosition(pen, true);	
		}
		counter++;
	}
}

bool GameInventory::IsOpened()
{
	return instance->opened;
}

std::list<InventorySlot*>* GameInventory::GetInventorySlots()
{
	return &instance->inventorySlots;
}

void GameInventory::CheckReturnDroppingObject()
{
}

void GameInventory::DrawInventorySlots()
{
	float2 inventoryBackgroundTopLeft = float2(instance->backgroundBB->GetMinPoint().x, instance->backgroundBB->GetMaxPoint().y);

	int startDrawingIndex = instance->currentPage * SLOTS_PER_PAGE;
	int endDrawingIndex = startDrawingIndex + SLOTS_PER_PAGE;

	int counter = 0;
	int drawCount = 0; 
	int slotsToDraw = 20 - startDrawingIndex; 

	float2 pen = inventoryBackgroundTopLeft + float2(40, 11);

	for (std::list<InventorySlot*>::iterator it = instance->inventorySlots.begin(); it != instance->inventorySlots.end(); it++)
	{
		if (counter < startDrawingIndex)
		{
			counter++;
			continue; 
		}
		else if (counter < endDrawingIndex)
		{
			// Draw Background 
			(*it)->GetSlotBB()->SetPosition(pen, true);
			(*it)->GetSlotBB()->Draw(true, instance->slotColor);
			pen.x += (*it)->GetSlotBB()->GetSize().x + instance->slotsInnerPadding;

			// Draw Object 
			FlyObject* objectInSlot = (*it)->GetSlotObject();
			if (objectInSlot)
			{
				if ((*it)->isObjectPicked)
				{
					instance->slotIconDropingAction->SetOwnPosition((*it)->GetSlotBB()->GetCenter());
					instance->slotIconDropingAction->Draw();
				}
				else
				{
					objectInSlot->transform->SetScale((*it)->slotScale);
					objectInSlot->DrawVisualLayer(true);
				//	flog("Draw Slot Object");
				}
			}

		}
		else
		{
			break;
		}
		
		counter++;
	}
}

FlyObject* GameInventory::PickObjectFromInventory(int index)
{
	if (!instance->opened || instance->droppingObject != nullptr)
		return nullptr; 

	int count = 0; 
	FlyObject* retObject = nullptr; 
	for (auto currentSlot = instance->inventorySlots.begin(); currentSlot != instance->inventorySlots.end(); currentSlot++)
	{
		if ((*currentSlot)->GetSlotObject() == nullptr)
			continue; 

		if (count == index)
		{	
			(*currentSlot)->isObjectPicked = true; 
			retObject = (*currentSlot)->GetSlotObject();
			retObject->transform->SetScale((*currentSlot)->viewportScale);
			break;
		}

		count++;
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

	instance->droppingObject->DrawVisualLayer(true);
	instance->droppingObject->DrawOverlaysLayer();

	//flog("Draw Object Dropping"); 
}

void GameInventory::DropObjectToRoom()
{
	for (auto it = instance->inventorySlots.begin(); it != instance->inventorySlots.end(); it++)
	{
		// TODO: this should be done by UID for seccurity
		if ((*it)->GetSlotObject() == nullptr)
			continue; 
		
		if (instance->droppingObject->GetUID() == (*it)->GetSlotObject()->GetUID())
		{
			(*it)->GetSlotObject()->isPicked = false; 

			Room* selectedRoom = App->moduleWorldManager->GetSelectedRoom(); 
			selectedRoom->AddFlyObject((*it)->GetSlotObject());

			(*it)->SetObject(nullptr); 
			(*it)->isObjectPicked = false;

			instance->droppingObject = nullptr; 

			break; 
		}
	}
}

BoundingBox* GameInventory::GetBackgroundBB()
{
	return instance->backgroundBB;
}

float GameInventory::GetMaxSlots()
{
	return maxSlots;
}

void GameInventory::SetMaxSlots(float _maxSlots)
{
	maxSlots = _maxSlots;
}

InventorySlot::InventorySlot(float inventoryBgHeigth)
{
	slotObject = nullptr;

	slotBB = new BoundingBox();
	slotBB->SetSize(inventoryBgHeigth - 20.0f, inventoryBgHeigth - 20.0f);

	slotScale = float2::one;
	viewportScale = float2::one;
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
	if (newObject != nullptr)
		slotObject = newObject;
	else
		slotObject = nullptr;	
}

bool InventorySlot::IsEmpty()
{
	if(slotObject == nullptr)
		return true;

	return false;
}

BoundingBox* InventorySlot::GetSlotBB()
{
	return slotBB;
}
