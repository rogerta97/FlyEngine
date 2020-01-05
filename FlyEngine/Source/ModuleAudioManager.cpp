#include "ModuleAudioManager.h"

ModuleAudioManager::ModuleAudioManager(bool start_enabled)
{
}

ModuleAudioManager::~ModuleAudioManager()
{
}

bool ModuleAudioManager::Init()
{
	//if (SDL_Init(SDL_INIT_AUDIO) < 0)
	//	return -1;

	return true;
}
