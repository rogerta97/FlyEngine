#ifndef _AUDIO_IMPORTER_H_
#define _AUDIO_IMPORTER_H_

#include <string>

class AudioClip;
class MusicTrack;
struct AudioImporter {

private:
	static AudioImporter* instance;
	AudioImporter();

public:
	static AudioImporter* getInstance();
	~AudioImporter();

	static void Delete();

	static AudioClip* LoadAudioClip(std::string audioPath); 
	static MusicTrack* LoadMusicTrack(std::string trackPath); 
};

#endif // !_IMAGE_IMPORTER_H_


