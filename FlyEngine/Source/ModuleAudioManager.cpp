#include "ModuleAudioManager.h"
#include "SDL.h"
#include "SDL_mixer.h"

#pragma comment(lib, "3rdParty/SDL_Mixer/libx86/SDL2_mixer.lib")

ModuleAudioManager::ModuleAudioManager(bool start_enabled)
{
	musicVolume = 50.0f;
	SFXVolume = 10.0f;
	currentMusic = nullptr; 
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

bool ModuleAudioManager::CleanUp()
{
	Mix_CloseAudio(); 

	return true;
}

float ModuleAudioManager::GetMusicVolume()
{
	return musicVolume;
}

void ModuleAudioManager::SetMusicVolume(float newVolume)
{
	musicVolume = newVolume; 
	Mix_VolumeMusic(musicVolume); 
}

float ModuleAudioManager::GetSFXVolume()
{
	return SFXVolume;
}

void ModuleAudioManager::SetSFXVolume(float newVolume)
{
	SFXVolume = newVolume; 
}
