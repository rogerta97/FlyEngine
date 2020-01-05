#include "AudioImporter.h"
#include "SDL_Mixer/SDL_mixer.h"

#include "AudioClip.h"
#include "MusicTrack.h"

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
	return new AudioClip(); 
}

MusicTrack* AudioImporter::LoadMusicTrack(std::string trackPath)
{
	return new MusicTrack();
}

void AudioImporter::Delete()
{
	delete instance; 
}
