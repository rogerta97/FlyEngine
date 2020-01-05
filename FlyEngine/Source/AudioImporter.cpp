#include "AudioImporter.h"

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

void AudioImporter::Delete()
{
	delete instance; 
}
