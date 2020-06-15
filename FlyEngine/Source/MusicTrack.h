#ifndef _MUSICTRACK_H_
#define _MUSICTRACK_H_

#include "Resource.h"
#include "SDL_mixer.h"

class MusicTrack : public Resource
{
public:
	MusicTrack();
	~MusicTrack();

	// Play 
	void Play(int loops = 0, bool forceNew = false);
	void Stop(); 

	// Set & Get 
	Mix_Music* GetMixMusic();
	void SetMixMusic(Mix_Music* newMixChunk);

	float GetAudioLenght();
	void SetAudioLenght(float newAudioLenght);

private:
	Mix_Music* mixMusic;
	float trackLenght;
};

#endif
