#include "AudioImporter.h"
#include "SDL_Mixer/SDL_mixer.h"
#include "Application.h"
#include "ModuleAudioManager.h"

#include "AudioClip.h"
#include "MusicTrack.h"

#include "mmgr.h"

AudioImporter* AudioImporter::instance = 0;

AudioImporter::AudioImporter()
{

}

AudioImporter* AudioImporter::getInstance()
{
	if (instance == 0)
		instance = new AudioImporter();

	return instance;
}

AudioImporter::~AudioImporter()
{
}

AudioClip* AudioImporter::LoadAudioClip(std::string audioPath)
{
	Mix_Chunk* audioChunk = Mix_LoadWAV(audioPath.c_str());

	if (audioChunk != nullptr)
	{
		Mix_VolumeChunk(audioChunk, App->moduleAudioManager->GetSFXVolume());
		AudioClip* newAudioClip = new AudioClip(); 
		newAudioClip->SetChunk(audioChunk); 
		newAudioClip->SetPath(audioPath.c_str()); 
		return newAudioClip;
	}

	return nullptr; 
}

MusicTrack* AudioImporter::LoadMusicTrack(std::string trackPath)
{
	Mix_Music* audioMixMusic = Mix_LoadMUS(trackPath.c_str());

	if (audioMixMusic != nullptr)
	{
		MusicTrack* newMusicTrack = new MusicTrack();
		newMusicTrack->SetMixMusic(audioMixMusic);
		newMusicTrack->SetPath(trackPath.c_str());
		return newMusicTrack;
	}

	return nullptr;
}

void AudioImporter::Delete()
{
	delete instance; 
}
