#include "MusicTrack.h"
#include "SDL_mixer.h"

MusicTrack::MusicTrack() : Resource(RESOURCE_MUSIC)
{
	mixMusic = nullptr;
	trackLenght = 0;
}

MusicTrack::~MusicTrack()
{

}

void MusicTrack::Play(int loops)
{

	if (Mix_PausedMusic())
	{
		Mix_ResumeMusic();
	}
	else
	{
		Mix_PlayMusic(mixMusic, loops);
	}
	
}

void MusicTrack::Stop()
{
	if (Mix_PlayingMusic())
		Mix_PauseMusic(); 
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
