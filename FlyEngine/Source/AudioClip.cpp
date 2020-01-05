#include "AudioClip.h"
#include "SDL_mixer.h"

AudioClip::AudioClip() : Resource(RESOURCE_SFX)
{
	mixChunk = nullptr; 
	audioLenght = 0; 
}

AudioClip::~AudioClip()
{

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
