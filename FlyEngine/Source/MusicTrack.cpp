#include "MusicTrack.h"
#include "SDL_mixer.h"
#include "Application.h"
#include "ModuleAudioManager.h"

#include "mmgr.h"

MusicTrack::MusicTrack() : Resource(RESOURCE_MUSIC)
{
	mixMusic = nullptr;
	trackLenght = 0;
	Mix_VolumeMusic(App->moduleAudioManager->GetMusicVolume()); 
}

MusicTrack::~MusicTrack()
{

}

void MusicTrack::Play(int loops, bool forceNew)
{
	if (Mix_PausedMusic())
	{
		Mix_ResumeMusic();
	}
	else
	{
		if(mixMusic != nullptr)
			Mix_PlayMusic(mixMusic, loops);
	}	
}

void MusicTrack::Stop()
{
	if (Mix_PlayingMusic())
	{
		Mix_HaltMusic();
	}
}

Mix_Music* MusicTrack::GetMixMusic()
{
	return mixMusic;
}

void MusicTrack::SetMixMusic(Mix_Music* newMixMusic)
{
	mixMusic = newMixMusic;
}

float MusicTrack::GetAudioLenght()
{
	return trackLenght;
}

void MusicTrack::SetAudioLenght(float newAudioLenght)
{
	trackLenght = newAudioLenght;
}
