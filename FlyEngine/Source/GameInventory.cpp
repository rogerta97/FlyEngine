#include "GameInventory.h"

GameInventory* GameInventory::instance = 0; 

GameInventory::GameInventory()
{
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
	for (auto currentObject = instance->objectsInInventory.begin(); currentObject != instance->objectsInInventory.end(); currentObject++)
	{
		if (count++ == index)
		{	
			FlyObject* retObject = *currentObject; 
			currentObject = instance->objectsInInventory.erase(currentObject);
			return retObject;
		}
	}

	return nullptr; 
}
