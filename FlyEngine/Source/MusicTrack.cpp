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
