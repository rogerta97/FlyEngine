#ifndef _AUDIO_IMPORTER_H_
#define _AUDIO_IMPORTER_H_

#include <string>

class Audio;
struct AudioImporter {

private:
	static AudioImporter* instance;
	AudioImporter();

public:
	static AudioImporter* getInstance();
	~AudioImporter();

	static void Delete();
};

#endif // !_IMAGE_IMPORTER_H_


