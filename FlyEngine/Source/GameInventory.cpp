#include "GameInventory.h"

#include "Application.h"
#include "ModuleImGui.h"
#include "GameViewportDockPanel.h"
#include "FlyObject.h"

#include "MathGeoLib.h"

GameInventory* GameInventory::instance = 0; 

GameInventory::GameInventory()
{
	droppingObject = nullptr; 
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

void GameInventory::AddObjectToInventoryList(FlyObject* newObject)
{
	if (newObject == nullptr)
		return; 

	instance->objectsInInventory.push_back(newObject); 
}

FlyObject* GameInventory::PickObjectFromInventory(int index)
{
	int count = 0; 
	FlyObject* retObject = nullptr; 
	for (auto currentObject = instance->objectsInInventory.begin(); currentObject != instance->objectsInInventory.end(); currentObject++)
	{
		if (count++ == index)
		{	
			retObject = *currentObject; 
			currentObject = instance->objectsInInventory.erase(currentObject);
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
	instance->droppingObject->transform->SetPosition(mouseRelativePos); 
	instance->droppingObject->Draw();

	FLY_LOG("Object Dropping"); 
}
