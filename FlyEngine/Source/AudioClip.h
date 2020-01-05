#ifndef _AUDIOCLIP_H_
#define _AUDIOCLIP_H_

#include "Resource.h"

class Mix_Chunk; 
class AudioClip : public Resource
{
public:
	AudioClip();
	~AudioClip();

	// Set & Get 
	Mix_Chunk* GetChunk();
	void SetChunk(Mix_Chunk* newMixChunk);

	float GetAudioLenght();
	void SetAudioLenght(float newAudioLenght);

private: 
	Mix_Chunk* mixChunk; 
	float audioLenght; 
};

#endif
