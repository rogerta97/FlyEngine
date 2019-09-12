#include "ModuleWorldManager.h"
#include "Room.h"

ModuleWorldManager::ModuleWorldManager(bool start_enabled)
{
	moduleType = MODULE_ENGINE_MANAGER;
}

ModuleWorldManager::~ModuleWorldManager()
{
}

bool ModuleWorldManager::Init()
{
	// Create fake rooms 
	Room* firstRoom = new Room(); 
	roomsInWorldList.push_back(firstRoom);

	return true;
}

update_status ModuleWorldManager::PreUpdate(float dt)
{
	return UPDATE_CONTINUE;
}

update_status ModuleWorldManager::PostUpdate(float dt)
{
	return UPDATE_CONTINUE;
}

bool ModuleWorldManager::CleanUp()
{
	return true;
}
