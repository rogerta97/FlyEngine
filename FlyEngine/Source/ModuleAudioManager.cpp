#include "ModuleAudioManager.h"
#include "SDL.h"
#include "SDL_mixer.h"

#pragma comment(lib, "3rdParty/SDL_Mixer/libx86/SDL2_mixer.lib")

ModuleAudioManager::ModuleAudioManager(bool start_enabled)
{
}

ModuleAudioManager::~ModuleAudioManager()
{
}

bool ModuleAudioManager::Init()
{
	if (SDL_Init(SDL_INIT_AUDIO) < 0)
		return false;

	if (Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 2, 4096) == -1)
		return false;
	
	return true;
}
