#include "AudioClip.h"
#include "SDL_mixer.h"

#include "Application.h"
#include "ModuleAudioManager.h"

#include "mmgr.h"

AudioClip::AudioClip() : Resource(RESOURCE_SFX)
{
	mixChunk = nullptr; 
	audioLenght = 0; 
}

AudioClip::~AudioClip()
{

}

int AudioClip::Play(int loops)
{
	Mix_VolumeChunk(mixChunk, App->moduleAudioManager->GetSFXVolume());
	return Mix_PlayChannel(-1, mixChunk, loops); 
}

Mix_Chunk* AudioClip::GetChunk()
{
	return mixChunk;
}

void AudioClip::SetChunk(Mix_Chunk* newMixChunk)
{
	mixChunk = newMixChunk;
}

float AudioClip::GetAudioLenght()
{
	return audioLenght;
}

void AudioClip::SetAudioLenght(float newAudioLenght)
{
	audioLenght = newAudioLenght; 
}
