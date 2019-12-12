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
