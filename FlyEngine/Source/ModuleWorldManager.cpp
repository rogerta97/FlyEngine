#include "ModuleWorldManager.h"

ModuleWorldManager::ModuleWorldManager(bool start_enabled)
{
	moduleType = MODULE_ENGINE_MANAGER;
}

ModuleWorldManager::~ModuleWorldManager()
{
}

bool ModuleWorldManager::Init()
{

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
