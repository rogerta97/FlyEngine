#include "ModuleAudioManager.h"
#include "SDL.h"
#include "SDL_mixer.h"

#pragma comment(lib, "3rdParty/SDL_Mixer/Libs/SDL_mixer.lib")

ModuleAudioManager::ModuleAudioManager(bool start_enabled)
{
}

ModuleAudioManager::~ModuleAudioManager()
{
}

bool ModuleAudioManager::Init()
{
	if (SDL_Init(SDL_INIT_AUDIO) < 0)
		return -1;
	
	return true;
}
